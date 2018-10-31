#include <vector>
#include <iostream>
#include <random>
#include <unordered_set>
#include <algorithm>
#include <cstddef>

#include "hello.decl.h"

#include "hello.h"
#include "main.decl.h"

extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ size_t numElements;
extern /* readonly */ size_t gridWidth;
extern /* readonly */ size_t gridHeight;
extern /* readonly */ size_t diameter;
extern /* readonly */ double duration;
extern /* readonly */ double reward;
extern /* readonly */ double penalty;

Hello::Hello() {

  exponential_distribution = std::exponential_distribution<double>(0.08);
  uniform_distribution = std::uniform_int_distribution<size_t>(0,std::numeric_limits<size_t>::max());

  timestamp = CkWallTimer();
  startTime = CkWallTimer();
  set_timestamp = CkWallTimer();

  my_x = thisIndex % gridWidth;
  my_y = thisIndex / gridWidth;

  channel = 1 + my_x / 5 + 100 * (my_y / 5);

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

  cur_set = new std::unordered_set<size_t>();
  bak_set = new std::unordered_set<size_t>();

}

// constructor for migration
Hello::Hello(CkMigrateMessage *msg) {}

void Hello::loop() {


  double current = CkWallTimer();

  if (current - startTime > duration) {
    mainProxy.done(thisIndex, stockpile);
    return;
  }

  double elapsed = current - timestamp;
  // CkPrintf("%f, %f\n",u
  //           current-startTime, elapsed);

  if (!elapsed) {
    thisProxy[thisIndex].loop();
    return;
  } else {
    timestamp = current;
  }

  if (current - set_timestamp > delay) {
    bak_set->clear();
    std::swap(bak_set, cur_set);
  }

  for (double draw = exponential_distribution(generator);
      draw < elapsed;
      elapsed -= draw, draw = exponential_distribution(generator)
    ) {

    // CkPrintf("hit!\n");
    for (size_t i = 0; i < NUM_NEIGHBORS; ++i) {
      if (channel) neighbors[i].takeTap(
          diameter,
          i,
          channel,
          uniform_distribution(generator)
        );
    }

  }

  thisProxy[thisIndex].loop();

}

void Hello::takeTap(size_t diam, size_t to_direction, size_t from_channel, size_t event_id) {

  // CkPrintf("tap %lu %lu\n",diam, to_direction);
  if (
      cur_set->find(from_channel) != cur_set->end() ||
      bak_set->find(from_channel) != bak_set->end() ||
      (channel && from_channel != channel))
    {
    return;
  }

  // CkPrintf("%f!\n", stockpile);
  stockpile += diam ? reward : penalty;
  cur_set->insert(event_id);

  if (to_direction == NORTH || to_direction == SOUTH) {
    neighbors[EAST].takeTap(diam ? diam-1 : diam, EAST, channel, event_id);
    neighbors[WEST].takeTap(diam ? diam-1 : diam, WEST, channel, event_id);
  }

  neighbors[to_direction].takeTap(diam ? diam-1 : diam, to_direction, channel, event_id);

}

#include "hello.def.h"
