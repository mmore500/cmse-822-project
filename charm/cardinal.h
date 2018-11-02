#ifndef __CARDINAL_H__
#define __CARDINAL_H__

struct Cardi {

  // cardinal directions
  enum Dir { SLF, N, S, E, W, NumDirs };

  // what's the opposite of each cardinal direction?
  static constexpr Dir Opp[] {SLF, S, N, W, E };

};

#endif //__CARDINAL_H__
