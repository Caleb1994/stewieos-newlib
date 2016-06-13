#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/times.h>
#include <sys/time.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <time.h>
#include <errno.h>

// **STUB** Retrieve process time
clock_t times(struct tms *buf)
{
	return 0;
}

// **STUB** Retrieve the actual world time
int gettimeofday(struct timeval *p, void*z)
{
	errno = EINVAL;
	return -1;
}