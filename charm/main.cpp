#include "main.decl.h"

#include "main.h"
#include "hello.decl.h"

/* readonly */
CProxy_Main mainProxy;

// entry point of Charm++ application
Main::Main(CkArgMsg* msg) {

  // initialize the local member variables
  doneCount = 0;

  // if a command line argument is supplied,
  // it is the number of chares to create.
  numElements = (msg->argc > 1) ? atoi(msg->argv[1]) : 5;

  // done with message, delete it
  delete msg;

  // display info about this execution
  CkPrintf("Running Hello World with %d elements "
            "using %d processors.\n",
            numElements, CkNumPes());

  // set the mainProxy reawdonly to point
  // to a proxy for the Main chare object
  // (i.e., this chare object)
  mainProxy = thisProxy;

  // create the array of Hello chare objects
  CProxy_Hello helloArray = CProxy_Hello::ckNew(numElements);

  // invoke the sayHi() entry method on all of the
  // elements in the helloArray array of chare objects
  helloArray.sayHi(-1);

}

// constructor for migration
Main::Main(CkMigrateMessage* msg) {}

// increment the doneCount
void Main::done() {
  // if everyone done, exit
  ++doneCount;

  // otherwise, keep waiting
  if (doneCount >= numElements) CkExit();
}

#include "main.def.h"
