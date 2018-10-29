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

public:

  /* Constructors */
  Hello();
  Hello(CkMigrateMessage *msg);

  /* Entry Methods */
  void sendTaps();
  void takeTap(size_t from_x, size_t from_y, size_t to_direction);

};

#endif //__HELLO_H__
