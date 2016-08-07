// util.c

#include "defs.h"

#ifdef WIN32
#include "windows.h"
#define getTimeMs GetTickCount
#else
#include <sys/time.h>
inline int getTimeMs() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec * 1000 + t.tv_usec / 1000;
}
#endif
