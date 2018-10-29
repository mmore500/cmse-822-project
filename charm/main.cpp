#include "main.decl.h"

#include "main.h"
#include "hello.decl.h"

/* readonly */ CProxy_Main mainProxy;
/* readonly */ size_t numElements;
/* readonly */ size_t gridWidth;
/* readonly */ size_t gridHeight;

// entry point of Charm++ application
Main::Main(CkArgMsg* msg) {

  // initialize the local member variables
  doneCount = 0;

  // if a command line argument is supplied,
  // it is the number of chares to create.
  gridWidth = (msg->argc > 2) ? atoi(msg->argv[1]) : 5;
  gridHeight = (msg->argc > 2) ? atoi(msg->argv[2]) : 5;
  numElements = gridWidth * gridHeight;

  // done with message, delete it
  delete msg;

  // display info about this execution
  CkPrintf("Running Hello World with %d elements "
            "using %d processors.\n",
            numElements, CkNumPes());

  // set the mainProxy readonly to point
  // to a proxy for the Main chare object
  // (i.e., this chare object)
  mainProxy = thisProxy;

  // create the array of Hello chare objects
  CProxy_Hello helloArray = CProxy_Hello::ckNew(numElements);

  // invoke the sendTaps() entry method on all of the
  // elements in the helloArray array of chare objects
  helloArray.sendTaps();

}

// constructor for migration
Main::Main(CkMigrateMessage* msg) {}

// increment the doneCount
void Main::done() {
  // if everyone done, exit
  ++doneCount;

  // otherwise, keep waiting
  if (doneCount >= 4*numElements) CkExit();
}

#include "main.def.h"
