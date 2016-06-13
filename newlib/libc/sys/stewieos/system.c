/*
* @Author: caleb
* @Date:   2016-05-30 19:45:19
* @Last Modified by:   caleb
* @Last Modified time: 2016-05-30 20:20:58
*/
#include <sys/syscall.h>
#include <errno.h>

void shutdown( void )
{
	int result = 0;
	SYSCALL0(SYSCALL_SHUTDOWN, result);
	errno = -result;
	return 0;
}