#include <vector>
#include <iostream>
#include <random>
#include <unordered_set>
#include <algorithm>
#include <cstddef>

#include "tile.decl.h"

#include "cardinal.h"
#include "tile.h"
#include "main.decl.h"

extern /* readonly */ CProxy_Main mainProxy;
extern /* readonly */ size_t numElements;
extern /* readonly */ size_t gridWidth;
extern /* readonly */ size_t gridHeight;
extern /* readonly */ size_t waveSize;
extern /* readonly */ double runDuration;
extern /* readonly */ double waveReward;
extern /* readonly */ double wavePenalty;

constexpr Cardi::Dir Cardi::Opp[];

Tile::Tile() {

  // set up random number machinery
  generator.seed(thisIndex);
  exp_dist = std::exponential_distribution<double>(0.18);
  uni_dist = std::uniform_int_distribution<size_t>(0,std::numeric_limits<size_t>::max());

  // who am I?
  x = thisIndex % gridWidth;
  y = thisIndex / gridWidth;

  channelID = 1 + x / 5 + 100 * (y / 5);
  CkPrintf("%lu!\n", channelID);

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

// constructor for migration
Tile::Tile(CkMigrateMessage *msg) {}

void Tile::seedGen(double lastSeedGenTime) {

  double curTime = CkWallTimer();

  double elapsedTime = curTime - lastSeedGenTime;

  // no-op if tile dead or no time elapsed
  if (elapsedTime && channelID) {

    // generate as many seed events as appropriate
    // given elapsed time and whim of RNG
    for (double draw = exp_dist(generator);
        draw < elapsedTime;
        elapsedTime -= draw, draw = exp_dist(generator)
      )
    {

      CkPrintf("seed!\n");

      // draw (probably) unique ID for seed event
      size_t seedID = uni_dist(generator);

      // send tap everywhere, including self
      for (size_t neigh = 0; neigh < neighbors.ckGetNumElements(); ++neigh) {
        neighbors[neigh].takeTap(
            waveSize,
            neigh,
            channelID,
            seedID
          );
      }

    }

  }

  // if the simulation is over, tell main we're done and stop
  // otherwise, queue next iteration of loop
  if (curTime > runDuration) {
    mainProxy.done(thisIndex, stockpile);
  } else {
    thisProxy[thisIndex].seedGen(curTime);
  }
}

void Tile::takeTap(
  size_t waveCountdown,
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
      this->channelID && // dead cells no propagate
      this->channelID == channelID && // only propagate if channels match
      curSeedIDs->find(seedID) == curSeedIDs->end() && // don't propagate ...
      bakSeedIDs->find(seedID) == bakSeedIDs->end() // ... if quiescent
    )
  {
    CkPrintf("tap!\n", stockpile);
    stockpile += waveCountdown ? waveReward : wavePenalty;
    curSeedIDs->insert(seedID);


    for (size_t neigh = 0; neigh < neighbors.ckGetNumElements(); ++neigh) {
      // if not the sender or self
      if (neigh == Cardi::Opp[outDirection] || neigh == Cardi::Dir::SLF) continue;

      // ... then forward the tap
      neighbors[neigh].takeTap(
          waveCountdown ? waveCountdown-1 : waveCountdown,
          neigh,
          channelID,
          seedID
        );
    }

  }

}

#include "tile.def.h"
