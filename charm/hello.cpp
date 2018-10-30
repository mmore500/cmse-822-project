#include <vector>
#include <iostream>
#include <random>

#include "hello.decl.h"

#include "hello.h"
#include "main.decl.h"

extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ size_t numElements;
extern /* readonly */ size_t gridWidth;
extern /* readonly */ size_t gridHeight;
extern /* readonly */ size_t diameter;
extern /* readonly */ double duration;

Hello::Hello() {

  distribution = std::exponential_distribution<double>(0.08);

  channel = 0;
  timestamp = CkWallTimer();
  startTime = CkWallTimer();

  my_x = thisIndex % gridWidth;
  my_y = thisIndex / gridWidth;

  std::vector<CkArrayIndex> neighbor_idxs(4);


  neighbor_idxs[NORTH] = CkArrayIndex(
    my_x
    + ((my_y + 1) % gridHeight) * gridWidth);

  neighbor_idxs[SOUTH] = CkArrayIndex(
    my_x
    + ((my_y + gridHeight - 1) % gridHeight) * gridWidth);

  neighbor_idxs[EAST] = CkArrayIndex(
    (my_x + 1) % gridWidth
    + my_y * gridWidth);

  neighbor_idxs[WEST] = CkArrayIndex(
    (my_x + gridWidth - 1) % gridWidth
    + my_y * gridWidth);

    neighbors = CProxySection_Hello::ckNew(thisProxy, neighbor_idxs);

}

// constructor for migration
Hello::Hello(CkMigrateMessage *msg) {}

void Hello::loop() {


  double current = CkWallTimer();

  if (current - startTime > duration) {
    mainProxy.done();
    return;
  }

  double elapsed = current - timestamp;
  // CkPrintf("%f, %f\n",
  //           current-startTime, elapsed);


  if (!elapsed) {
    thisProxy[thisIndex].loop();
    return;
  } else {
    timestamp = current;
  }

  for (double draw = distribution(generator);
      draw < elapsed;
      elapsed -= draw, draw = distribution(generator)
    ) {

    CkPrintf("hit!\n");
    for (size_t i = 0; i < NUM_NEIGHBORS; ++i) {
      neighbors[i].takeTap(diameter, i);
    }

  }

  thisProxy[thisIndex].loop();

}

void Hello::takeTap(size_t diam, size_t to_direction) {

  // CkPrintf("tap %lu %lu\n",diam, to_direction);

  if (diam > 0) {
    if (to_direction == NORTH || to_direction == SOUTH) {
      neighbors[EAST].takeTap(diam-1, EAST);
      neighbors[WEST].takeTap(diam-1, WEST);
    }

    neighbors[to_direction].takeTap(diam-1, to_direction);

  }

}

#include "hello.def.h"
