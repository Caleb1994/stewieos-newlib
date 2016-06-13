/*
* @Author: caleb
* @Date:   2016-05-30 19:45:16
* @Last Modified by:   caleb
* @Last Modified time: 2016-05-30 20:20:51
*/
#include <sys/module.h>
#include <sys/syscall.h>
#include <errno.h>

// insert a kernel module
int insmod(const char* module)
{
	int result;
	SYSCALL1(SYSCALL_INSMOD, result, module);
	errno = -result;
	if( result < 0 ){
		return -1;
	}
	return 0;
}

// remove a kernel module
int rmmod(const char* module)
{
	int result;
	SYSCALL1(SYSCALL_RMMOD, result, module);
	errno = -result;
	if( result < 0 ){
		return -1;
	}
	return 0;
} 
