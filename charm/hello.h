#ifndef __HELLO_H__
#define __HELLO_H__

class Hello : public CBase_Hello {

private:
  const size_t NORTH = 0;
  const size_t SOUTH = 1;
  const size_t EAST = 2;
  const size_t WEST = 3;

  const size_t NUM_NEIGHBORS = 4;

  CProxySection_Hello neighbors;
  int channel;

  size_t my_x;
  size_t my_y;

  double timestamp;
  double startTime;

  std::default_random_engine generator;
  std::exponential_distribution<double> distribution;

public:

  /* Constructors */
  Hello();
  Hello(CkMigrateMessage *msg);

  /* Entry Methods */
  void loop();
  void takeTap(size_t diameter, size_t to_direction);

};

#endif //__HELLO_H__
