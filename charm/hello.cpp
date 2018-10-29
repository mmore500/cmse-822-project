#include <vector>

#include "hello.decl.h"

#include "hello.h"
#include "main.decl.h"

extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ size_t numElements;
extern /* readonly */ size_t gridWidth;
extern /* readonly */ size_t gridHeight;

Hello::Hello() {

  channel = 0;

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

void Hello::sendTaps() {

  // have this chare object say hello
  CkPrintf("Sending taps from chare # %d on "
            "processor %d.\n",
            thisIndex, CkMyPe());

  for (size_t i = 0; i < NUM_NEIGHBORS; ++i) {
    neighbors[i].takeTap(my_x, my_y, i);
  }

}

void Hello::takeTap(size_t from_x, size_t from_y, size_t to_direction) {

  // have this chare object report a tap
  CkPrintf("%lu, %lu reporting tap from %lu,%lu to direction %lu.\n",
            my_x, my_y, from_x, from_y, to_direction);

  // report to the Main chare object that this chare object
  // has completed its task
  mainProxy.done();

}

#include "hello.def.h"
