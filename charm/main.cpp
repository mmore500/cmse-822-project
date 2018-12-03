#include <sstream>
#include <string>
#include <stdlib.h>
#include <fstream>

#include "main.decl.h"

#include "config/command_line.h"
#include "config/ArgManager.h"

#include "Config.h"

#include "main.h"
#include "tile.decl.h"

/* readonly */ CProxy_Main mainProxy;

/* WORLD_STRUCTURE */
/* reqdonly */ size_t GRID_WIDTH;
/* readonly */ size_t GRID_HEIGHT;

/* RUN_STRUCTURE */
/* readonly */ double RUN_DURATION;
/* readonly */ double WAVE_DURATION;

/* RESOURCE_STRUCTURE */
/* readonly */ double WAVE_SIZE;
/* readonly */ double WAVE_REWARD;
/* readonly */ double WAVE_PENALTY;

// entry point of Charm++ application
Main::Main(CkArgMsg* msg) {

  Config config;

  config.Read("Config.cfg", false);
  auto args = emp::cl::ArgManager(msg->argc, msg->argv);

  if (args.ProcessConfigOptions(config, std::cout, "OpenWorld.cfg", "OpenWorld-macros.h") == false) CkExit();
  if (args.TestUnknown() == false) CkExit();  // If there are leftover args, throw an error.

  /* WORLD_STRUCTURE */
  GRID_WIDTH = config.GRID_WIDTH();
  GRID_HEIGHT = config.GRID_HEIGHT();

  /* RUN_STRUCTURE */
  RUN_DURATION = config.RUN_DURATION();
  WAVE_DURATION = config.WAVE_DURATION();

  /* RESOURCE_STRUCTURE */
  WAVE_SIZE = config.WAVE_SIZE();
  WAVE_REWARD = config.WAVE_REWARD();
  WAVE_PENALTY = config.WAVE_PENALTY();

  // done with message, delete it
  delete msg;

  // initialize the local member variables
  doneCount = 0;
  numElements = GRID_WIDTH * GRID_HEIGHT;

  // display info about this execution
  CkPrintf("Running Tile World with %d elements "
            "using %d processors.\n",
            numElements, CkNumPes());

  // set the mainProxy readonly to point
  // to a proxy for the Main chare object
  // (i.e., this chare object)
  mainProxy = thisProxy;

  // create the array of Tile chare objects
  CProxy_Tile tileArray = CProxy_Tile::ckNew(numElements);


  std::ostringstream oss;
  oss << "../layouts/" << GRID_WIDTH << "x" << GRID_HEIGHT << ".csv";

  std::ifstream  data(oss.str());
  std::string line;
  std::vector<std::vector<int> > parsedCsv;
  while(std::getline(data,line))
  {
      std::stringstream lineStream(line);
      std::string cell;
      std::vector<int> parsedRow;
      while(std::getline(lineStream,cell,','))
      {
          parsedRow.push_back(atoi(cell.c_str()));
      }

      parsedCsv.push_back(parsedRow);
  }

  for( size_t x = 0; x < GRID_WIDTH; ++x) {
    for( size_t y = 0; y < GRID_HEIGHT; ++y) {
      tileArray[x+y*GRID_WIDTH].setChan(parsedCsv[y][x]);
    }
  }

  // invoke the seedGen() entry method on all of the
  // elements in the tileArray array of chare objects
  tileArray.seedGen(0.0);

}

// constructor for migration
Main::Main(CkMigrateMessage* msg) {}

// increment the doneCount
void Main::done(size_t which, double amount) {
  // if everyone done, exit
  ++doneCount;

  result[which] = amount;

  // otherwise, keep waiting
  if (doneCount >= numElements) {

    CkPrintf("test1");
    std::fstream fs;
    fs.open("output.csv", std::fstream::out);

    for (size_t x = 0; x < GRID_WIDTH; ++x) {
      for (size_t y = 0; y < GRID_HEIGHT; ++y) {
        fs << result[x+y*GRID_WIDTH];
        if(y<GRID_HEIGHT-1) fs <<",";
      }
      fs << '\n';
    }

    CkPrintf("test");
    fs.close();
    CkExit();
  }
}

#include "main.def.h"
