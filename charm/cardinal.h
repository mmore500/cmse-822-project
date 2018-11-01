#ifndef __CARDINAL_H__
#define __CARDINAL_H__

struct Cardi {

  enum Dir { SLF, N, S, E, W, NumDirs };

  static constexpr Dir Opp[] {SLF, S, N, W, E };

};

#endif //__CARDINAL_H__
