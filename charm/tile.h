#ifndef __HELLO_H__
#define __HELLO_H__

class Tile : public CBase_Tile {

private:
  const size_t NORTH = 0;
  const size_t SOUTH = 1;
  const size_t EAST = 2;
  const size_t WEST = 3;

  const size_t NUM_NEIGHBORS = 4;

  CProxySection_Tile neighbors;
  size_t channel;

  size_t my_x;
  size_t my_y;

  double timestamp;
  double startTime;

  std::default_random_engine generator;
  std::exponential_distribution<double> exponential_distribution;
  std::uniform_int_distribution<size_t> uniform_distribution;
  double stockpile = 0.0;

  double delay = 5.0;

  double set_timestamp;

  std::unordered_set<size_t> *cur_set;
  std::unordered_set<size_t> *bak_set;

public:

  /* Constructors */
  Tile();
  Tile(CkMigrateMessage *msg);

  /* Entry Methods */
  void loop();
  void takeTap(size_t diameter, size_t to_direction, size_t from_channel, size_t event_id);

};

#endif //__HELLO_H__
