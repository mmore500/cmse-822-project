#ifndef __HELLO_H__
#define __HELLO_H__

class Tile : public CBase_Tile {

private:

  CProxySection_Tile neighbors;

  /* Who am I? */
  size_t channelID;
  size_t x;
  size_t y;

  /* RNG stuff */
  std::mt19937 generator;
  std::exponential_distribution<double> exp_dist;
  std::uniform_int_distribution<size_t> uni_dist;

  /* Resource collection */
  double stockpile = 0.0;

  /* Manage quiescent period */
  double lastSetSwapTime = 0.0; // in seconds

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
      double firstTapTime,
      size_t firstTapX,
      size_t firstTapY,
      size_t outDirection,
      size_t channelID,
      size_t seedID
    );

};

#endif //__HELLO_H__
