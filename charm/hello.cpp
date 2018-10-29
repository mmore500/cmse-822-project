#include "hello.decl.h"

#include "hello.h"
#include "main.decl.h"

extern /* readonly */ CProxy_Main mainProxy;

Hello::Hello() {
  // no-op
}

// constructor for migration
Hello::Hello(CkMigrateMessage *msg) {}

void Hello::sayHi(int from) {

  // have this chare object say hello
  CkPrintf("Hello from Hello chare # %d on "
            "processor %d (told by %d).\n",
            thisIndex, CkMyPe(), from);

  // report to the Main chare object that this chare object
  // has completed its task
  mainProxy.done();

}

#include "hello.def.h"
