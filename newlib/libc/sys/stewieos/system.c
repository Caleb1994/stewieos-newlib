/*
* @Author: caleb
* @Date:   2016-05-30 19:45:19
* @Last Modified by:   Caleb Stewart
* @Last Modified time: 2016-06-26 00:01:25
*/
#include <sys/syscall.h>
#include <errno.h>
#include <stdarg.h>

void shutdown( void )
{
	int result = 0;
	SYSCALL0(SYSCALL_SHUTDOWN, result);
	errno = -result;
	return 0;
}

void syslog( int level, const char* format, ...)
{
	char buffer[1024];
	va_list va;
	int result = 0;

	// Format the output
	va_start(va, format);
	vsnprintf(buffer, 1024, format, va);
	va_end(va);

	// Execute the system call
	SYSCALL2(SYSCALL_SYSLOG, result, level, buffer);

	return;
}