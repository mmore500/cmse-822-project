#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <mpi.h>
#include <vector>
#include <assert.h>
#include <cmath>
#include <algorithm>

#include "config/command_line.h"
#include "config/ArgManager.h"

#include "Config.h"
#include "cardinal.h"
#include "distance.h"
#include "get_walltime.h"

int main(int argc, char *argv[])
{

  Config config;

  config.Read("Config.cfg", false);
  auto args = emp::cl::ArgManager(argc, argv);

  if (args.ProcessConfigOptions(config, std::cout, "OpenWorld.cfg", "OpenWorld-macros.h") == false) return 1;
  if (args.TestUnknown() == false) return 1;  // If there are leftover args, throw an error.

  // Initialize the MPI environment
  MPI_Init(&argc, &argv);

  size_t waveskip = (int)(2*config.WAVE_SIZE()+1);
  std::mt19937 eng(1); // seed the generator
  std::uniform_int_distribution<> distr(0, waveskip-1); // define the range

  /* SAFETY CHECKS */
  // Get the number of processes
  int world_size;
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  // should be a square number
  assert(sqrt(world_size)*sqrt(world_size)==world_size);

  // assume grid is square
  assert(config.GRID_WIDTH()==config.GRID_HEIGHT());

  // assume procs evenly divide dimension
  assert(config.GRID_WIDTH()%(int)sqrt(world_size)==0);

  int dims [2] = {(int)sqrt(world_size), (int)sqrt(world_size)};
  int periods [2] = {true, true};

  // make a cartesian grid
  MPI_Comm comm_cart;
  MPI_Cart_create(
    MPI_COMM_WORLD, /* MPI_Comm comm_old */
    2, /* int ndims */
    dims, /* int *dims */
    periods, /* int *periods */
    true , /* int reorder */
    &comm_cart /* MPI_Comm *comm_cart */
  );

  // Get the rank of the process
  int world_rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  int coord_2d [2];
  MPI_Cart_coords(
    comm_cart, /* MPI_Comm comm */
    world_rank, /* int rank */
    2, /* int maxdims */
    coord_2d /* int *coords */
  );

  int rank_2d;
  MPI_Cart_rank(
    comm_cart, /* MPI_Comm comm */
    coord_2d, /* int *coords */
    &rank_2d /* int *rank */
  );

  printf("I am %d: (%d,%d); originally %d\n",rank_2d,coord_2d[0],coord_2d[1],world_rank);

  int rank_right, rank_left, rank_up, rank_down;

  MPI_Cart_shift(
    comm_cart, /* MPI_Comm comm */
    0, /* int direction */
    +1, /* int displ */
    &rank_2d, /* int *source */
    &rank_right /* int *dest */
  );
  MPI_Cart_shift(
    comm_cart, /* MPI_Comm comm */
    0, /* int direction */
    -1, /* int displ */
    &rank_2d, /* int *source */
    &rank_left /* int *dest */
  );
  MPI_Cart_shift(
    comm_cart, /* MPI_Comm comm */
    1, /* int direction */
    +1, /* int displ */
    &rank_2d, /* int *source */
    &rank_up /* int *dest */
  );
  MPI_Cart_shift(
    comm_cart, /* MPI_Comm comm */
    1, /* int direction */
    -1, /* int displ */
    &rank_2d, /* int *source */
    &rank_down /* int *dest */
  );

  // set up own mini-grid
  const size_t mini_grid_dim = config.GRID_WIDTH()/(int)sqrt(world_size);
  const size_t rel_x = mini_grid_dim * coord_2d[0];
  const size_t rel_y = mini_grid_dim * coord_2d[1];

  // setup data types
  MPI_Datatype verticalEdge;
  MPI_Type_vector(
    mini_grid_dim, /* int count */
    1, /* int blocklength */
    mini_grid_dim, /* int stride */
    MPI_INT, /* MPI_Datatype oldtype */
    &verticalEdge /* MPI_Datatype *newtype */
  );
  MPI_Type_commit(&verticalEdge);

  // setup channel IDs
  size_t* channelIDs = new size_t[mini_grid_dim*mini_grid_dim];

  // setup resource stockpiles
  double* stockpiles = new double[mini_grid_dim*mini_grid_dim];

  // setup signal status
 int* signals = new int[mini_grid_dim*mini_grid_dim];
 int* signals_bak = new int[mini_grid_dim*mini_grid_dim];

 int* signals_up = new int[mini_grid_dim];
 int* signals_down = new int[mini_grid_dim];
 int* signals_left = new int[mini_grid_dim];
 int* signals_right = new int[mini_grid_dim];

  // initialize everything
  for(size_t i = 0; i < mini_grid_dim*mini_grid_dim; ++i) {
      channelIDs[i] = 1;
      stockpiles[i] = 0;
      signals[i] = -1; // nothing going on
      signals_bak[i] = -1; // nothing going on
  }

  MPI_Request send_requests [4];
  MPI_Request recv_requests [4];

  // do work
  double target_walltime = get_walltime() + config.RUN_DURATION();
  int bcast_breaker = true;
  for(size_t update = 0; bcast_breaker; ++update) {
    // handle periphery send/recieve
    // left
    MPI_Isend(
      signals, /* const void *buf */
      1, /* int count */
      verticalEdge, /* MPI_Datatype datatype */
      rank_left, /* int dest */
      0, /* int tag */
      comm_cart, /* MPI_Comm comm */
      &send_requests[0] /* MPI_Request *request */
    );
    MPI_Irecv(
      signals_left, /* void *buf */
      mini_grid_dim, /* int count */
      MPI_INT, /* MPI_Datatype datatype */
      rank_left, /* int source */
      1, /* int tag */
      comm_cart, /* MPI_Comm comm */
      &recv_requests[0] /* MPI_Request *request */
    );

    // right
    MPI_Isend(
      &signals[mini_grid_dim-1], /* const void *buf */
      1, /* int count */
      verticalEdge, /* MPI_Datatype datatype */
      rank_right, /* int dest */
      1, /* int tag */
      comm_cart, /* MPI_Comm comm */
      &send_requests[1] /* MPI_Request *request */
    );
    MPI_Irecv(
      signals_right, /* void *buf */
      mini_grid_dim, /* int count */
      MPI_INT, /* MPI_Datatype datatype */
      rank_right, /* int source */
      0, /* int tag */
      comm_cart, /* MPI_Comm comm */
      &recv_requests[1] /* MPI_Request *request */
    );

    //`up
    MPI_Isend(
      &signals[mini_grid_dim*(mini_grid_dim-1)], /* const void *buf */
      mini_grid_dim, /* int count */
      MPI_INT, /* MPI_Datatype datatype */
      rank_up, /* int dest */
      2, /* int tag */
      comm_cart, /* MPI_Comm comm */
      &send_requests[2] /* MPI_Request *request */
    );
    MPI_Irecv(
      signals_up, /* void *buf */
      mini_grid_dim, /* int count */
      MPI_INT, /* MPI_Datatype datatype */
      rank_up, /* int source */
      3, /* int tag */
      comm_cart, /* MPI_Comm comm */
      &recv_requests[2] /* MPI_Request *request */
    );


    // down
    MPI_Isend(
      signals, /* const void *buf */
      mini_grid_dim, /* int count */
      MPI_INT, /* MPI_Datatype datatype */
      rank_down, /* int dest */
      3, /* int tag */
      comm_cart, /* MPI_Comm comm */
      &send_requests[3] /* MPI_Request *request */
    );
    MPI_Irecv(
      signals_down, /* void *buf */
      mini_grid_dim, /* int count */
      MPI_INT, /* MPI_Datatype datatype */
      rank_down, /* int source */
      2, /* int tag */
      comm_cart, /* MPI_Comm comm */
      &recv_requests[3] /* MPI_Request *request */
    );

    // handle interior sending/receiving
    for(size_t x = 0; x < mini_grid_dim; ++x) {
      for(size_t y = 0; y < mini_grid_dim; ++y) {
        if(signals[x+y*mini_grid_dim] >= 0) {
          signals_bak[x+y*mini_grid_dim] = -2;
          for(int dx : {x > 0 ? -1 : 0, x < mini_grid_dim-1 ? 1 : 0}) {
            for(int dy : {y > 0 ? -1 : 0, y < mini_grid_dim-1 ? 1 : 0}) {
              if (signals_bak[x+dx+(y+dy)*mini_grid_dim] == -1) {
                signals_bak[x+dx+(y+dy)*mini_grid_dim] = signals[x+y*mini_grid_dim]+1;
              } else if (signals_bak[x+dx+(y+dy)*mini_grid_dim] >= 0) {
                signals_bak[x+dx+(y+dy)*mini_grid_dim] =
                std::min(
                  signals[x+y*mini_grid_dim]+1,
                  signals_bak[x+dx+(y+dy)*mini_grid_dim]
                );
              }
            }
          }
          // distribute reward
          if(signals[x+y*mini_grid_dim] >= config.WAVE_SIZE()) {
            stockpiles[x+y*mini_grid_dim] += config.WAVE_REWARD();
          } else {
            stockpiles[x+y*mini_grid_dim] += config.WAVE_PENALTY();
          }
        }
      }
    }

    MPI_Waitall(
      4, /* int count */
      recv_requests, /* MPI_Request array_of_requests[] */
      MPI_STATUSES_IGNORE /* MPI_Status array_of_statuses[] */
    );

    // handle periphery receiving and processing

    // left
    for(size_t y = 0; y < mini_grid_dim; ++y) {
      if (signals_left[y] >= 0) {
        if (signals_bak[0+y*mini_grid_dim] == -1) {
          //TODO order ambiguity here
          signals_bak[0+y*mini_grid_dim] = signals_left[y]+1;
        }
      }
    }

    // right
    for(size_t y = 0; y < mini_grid_dim; ++y) {
      if (signals_right[y] >= 0) {
        if (signals_bak[mini_grid_dim-1+y*mini_grid_dim] == -1) {
          //TODO order ambiguity here
          signals_bak[mini_grid_dim-1+y*mini_grid_dim] = signals_right[y]+1;
        }
      }
    }

    // up
    for(size_t x = 0; x < mini_grid_dim; ++x) {
      if (signals_up[x] >= 0) {
        if (signals_bak[mini_grid_dim*(mini_grid_dim-1)+x] == -1) {
          //TODO order ambiguity here
          signals_bak[mini_grid_dim*(mini_grid_dim-1)+x] = signals_up[x]+1;
        }
      }
    }

    // bottom
    for(size_t x = 0; x < mini_grid_dim; ++x) {
      if (signals_down[x] >= 0) {
        if (signals_bak[x] == -1) {
          //TODO order ambiguity here
          signals_bak[x] = signals_down[x]+1;
        }
      }
    }

    // seed events
    if (update % (1+(int)config.WAVE_SIZE()) == 0) {

      // seed new
      size_t seed_disp_x = distr(eng);
      size_t seed_disp_y = distr(eng);

      // how far back is the edge?
      size_t buffx = (waveskip - rel_x % waveskip)%waveskip;
      size_t buffy = (waveskip - rel_y % waveskip)%waveskip;

      size_t startx = seed_disp_x >= buffx ? seed_disp_x - buffx : seed_disp_x + waveskip - buffx;
      size_t starty = seed_disp_y >= buffy ? seed_disp_y - buffy : seed_disp_y + waveskip - buffy;

      for(size_t x = startx; x < mini_grid_dim; x+=waveskip) {
        for(size_t y = starty; y < mini_grid_dim; y+=waveskip) {
          if (signals[x+y*mini_grid_dim] == -1) signals_bak[x+y*mini_grid_dim] = 0;
        }
      }
    }

    MPI_Waitall(
      4, /* int count */
      send_requests, /* MPI_Request array_of_requests[] */
      MPI_STATUSES_IGNORE /* MPI_Status array_of_statuses[] */
    );

    // TODO swap signals and signals_bak
    std::swap(signals, signals_bak);

    bcast_breaker = target_walltime > get_walltime();
    MPI_Bcast(
      &bcast_breaker, /* void *buffer */
      1, /* int count */
      MPI_INT, /* MPI_Datatype datatype*/
      0, /* int root */
      comm_cart /* MPI_Comm comm */
    );
  }

  // Finalize the MPI environment.
  MPI_Type_free(&verticalEdge);
  MPI_Finalize();


  std::cout << "hello world" << std::endl;
  return 0;
}
