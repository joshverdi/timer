#ifndef GETTIMEOFDAY_H
#define GETTIMEOFDAY_H

#if defined(_MSC_VER) || defined(_MSC_EXTENSIONS)
#   include <time.h>
#   include <windows.h>
#   define DELTA_EPOCH_IN_MICROSECS  11644473600000000Ui64

//struct timeval {
//    long tv_sec;
//    long tv_usec;
//};

struct timezone
{
  int  tz_minuteswest; /* minutes W of Greenwich */
  int  tz_dsttime;     /* type of dst correction */
};

int gettimeofday(struct timeval *tv, struct timezone *tz);

#else
#   include <sys/time.h>
#endif

#endif // GETTIMEOFDAY_H
