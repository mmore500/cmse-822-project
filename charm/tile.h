#ifndef __HELLO_H__
#define __HELLO_H__

class Tile : public CBase_Tile {

private:

  CProxySection_Tile neighbors;

  size_t channelID;

  size_t x;
  size_t y;

  std::mt19937 generator;
  std::exponential_distribution<double> exp_dist;
  std::uniform_int_distribution<size_t> uni_dist;
  double stockpile = 0.0;


  double setSwapDelay = 5.0;
  double lastSetSwapTime = 0.0;

  std::unordered_set<size_t> seedIDSets[2];
  std::unordered_set<size_t> *curSeedIDs;
  std::unordered_set<size_t> *bakSeedIDs;

public:

  /* Constructors */
  Tile();
  Tile(CkMigrateMessage *msg);

  /* Entry Methods */
  void seedGen(double lastSeedGenTime);
  void takeTap(
      size_t waveCountdown,
      size_t outDirection,
      size_t channelID,
      size_t event_id
    );

};

#endif //__HELLO_H__
