#include <vector>
#include <iostream>
#include <random>
#include <unordered_set>
#include <algorithm>
#include <cstddef>

#include "tile.decl.h"

#include "cardinal.h"
#include "distance.h"
#include "tile.h"
#include "main.decl.h"

extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ size_t numElements;
extern /* readonly */ size_t gridWidth;
extern /* readonly */ size_t gridHeight;
extern /* readonly */ double waveSize;
extern /* readonly */ double runDuration;
extern /* readonly */ double waveReward;
extern /* readonly */ double wavePenalty;

// RE: cardinal directions
// necessary for successful compile
constexpr Cardi::Dir Cardi::Opp[];

/* main constuctor */
Tile::Tile() {

  // set up random number machinery
  generator.seed(thisIndex);
  exp_dist = std::exponential_distribution<double>(0.18);
  uni_dist = std::uniform_int_distribution<size_t>(0,std::numeric_limits<size_t>::max());

  // who am I?
  x = thisIndex % gridWidth;
  y = thisIndex / gridWidth;

  // TODO temporary hack for testing purposes
  channelID = 1 + x / 5 + 100 * (y / 5);

  // set up pointers for cur/bak swapping
  curSeedIDs = &seedIDSets[0];
  bakSeedIDs = &seedIDSets[1];

  // set up neighbors
  std::vector<CkArrayIndex> neighborIdxs(Cardi::Dir::NumDirs);

  neighborIdxs[Cardi::Dir::SLF] = CkArrayIndex(thisIndex);

  neighborIdxs[Cardi::Dir::N] = CkArrayIndex(
    x
    + ((y + 1) % gridHeight) * gridWidth);

  neighborIdxs[Cardi::Dir::S] = CkArrayIndex(
    x
    + ((y + gridHeight - 1) % gridHeight) * gridWidth);

  neighborIdxs[Cardi::Dir::E] = CkArrayIndex(
    (x + 1) % gridWidth
    + y * gridWidth);

  neighborIdxs[Cardi::Dir::W] = CkArrayIndex(
    (x + gridWidth - 1) % gridWidth
    + y * gridWidth);

  neighbors = CProxySection_Tile::ckNew(thisProxy, neighborIdxs);

}

/* constructor for migration */
Tile::Tile(CkMigrateMessage *msg) {/* TODO */}

/* Seed resource waves */
void Tile::seedGen(double lastSeedGenTime) {

  double curTime = CkWallTimer();

  double elapsedTime = curTime - lastSeedGenTime;

  // no-op if tile dead or no time elapsed
  if (elapsedTime && channelID) {

    // generate as many seed events as appropriate ...
    // ... given elapsed time and whim of RNG
    for (double draw = exp_dist(generator);
        draw < elapsedTime;
        elapsedTime -= draw, draw = exp_dist(generator)
      )
    {

      // CkPrintf("seed!\n");

      // draw (probably) unique ID for seed event
      size_t seedID = uni_dist(generator);

      // send tap everywhere, including self
      for (size_t neigh = 0; neigh < neighbors.ckGetNumElements(); ++neigh) {
        neighbors[neigh].takeTap(
            curTime,
            x,
            y,
            neigh,
            channelID,
            seedID
          );
      }

    }

  }

  // if the simulation is over, tell main we're done and stop
  // otherwise, queue next iteration of wave seeding loop
  if (curTime > runDuration) {
    mainProxy.done(thisIndex, stockpile);
  } else {
    thisProxy[thisIndex].seedGen(curTime);
  }

}

/*
 * Resource wave propagation.
 */
void Tile::takeTap(
  double firstTapTime,
  size_t firstTapX,
  size_t firstTapY,
  size_t outDirection,
  size_t channelID,
  size_t seedID
) {

  // check if we need to swap curSeedIDs and bakSeedIDs
  double curTime = CkWallTimer();
  if (curTime - lastSetSwapTime > setSwapDelay) {
    lastSetSwapTime = curTime;
    bakSeedIDs->clear();
    std::swap(curSeedIDs, bakSeedIDs);
  }

  if (
      this->channelID && // only propagate if not dead
      this->channelID == channelID && // only propagate if channels match
      curSeedIDs->find(seedID) == curSeedIDs->end() && // only propagate ...
      bakSeedIDs->find(seedID) == bakSeedIDs->end() && // ... if not quiescent
      curTime - firstTapTime < setSwapDelay // could outlast quiescence, kill
    )
  {

    // register wave efefct on resource stockpile
    double d = distance(x, y, firstTapX, firstTapY, gridWidth, gridHeight);
    stockpile += d <= waveSize ? waveReward : wavePenalty;

    // enter quiescence for this particular wave
    curSeedIDs->insert(seedID);

    // propagate wave
    for (size_t neigh = 0; neigh < neighbors.ckGetNumElements(); ++neigh) {
      if (
          neigh == Cardi::Opp[outDirection] || // if the sender ...
          neigh == Cardi::Dir::SLF // ... or self ...
        ) continue; // ... then no-op

      // ... otherwise, forward the tap
      neighbors[neigh].takeTap(
          firstTapTime,
          firstTapX,
          firstTapY,
          neigh,
          channelID,
          seedID
        );
    }

  }

}

#include "tile.def.h"
