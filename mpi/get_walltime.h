#ifndef __GET_WALLTIME_H__
#define __GET_WALLTIME_H__

#include <stddef.h>
#include <sys/time.h>

double get_walltime_() {
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return (double)(tp.tv_sec + tp.tv_usec/1000000.0);
}

double get_walltime() {
  return get_walltime_();
}

#endif //__GET_WALLTIME_H__
