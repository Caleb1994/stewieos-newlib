#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <stdio.h>
#include <errno.h>

// Increase the allocated process memory
void* sbrk(ptrdiff_t incr)
{
	int result;
	SYSCALL1(SYSCALL_SBRK, result, incr);
	return (caddr_t)result;
}
