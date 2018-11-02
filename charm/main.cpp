#include "main.decl.h"

#include "main.h"
#include "tile.decl.h"

/* readonly */ CProxy_Main mainProxy;
/* readonly */ size_t numElements;
/* readonly */ size_t gridWidth;
/* readonly */ size_t gridHeight;
/* readonly */ double runDuration;
/* readonly */ double waveSize;
/* readonly */ double waveReward;
/* readonly */ double wavePenalty;

// entry point of Charm++ application
Main::Main(CkArgMsg* msg) {

  // initialize the local member variables
  doneCount = 0;
  waveSize = 2.0;
  waveReward = 1;
  wavePenalty = -5;

  // if a command line argument is supplied,
  // it is the number of chares to create.
  gridWidth = (msg->argc > 2) ? atoi(msg->argv[1]) : 5;
  gridHeight = (msg->argc > 2) ? atoi(msg->argv[2]) : 5;
  numElements = gridWidth * gridHeight;
  runDuration = (msg->argc > 3) ? atoi(msg->argv[3]) : 15;

  // done with message, delete it
  delete msg;

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

    for (size_t i = 0; i < numElements; ++i) {
      CkPrintf("%lu %f\n", i, result[i]);
    }

    CkExit();
  }
}

#include "main.def.h"
