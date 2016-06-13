#define _GNU_SOURCE
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/wait.h>
#include <sys/message.h>
#include <stdio.h>
#include <errno.h>

// Kill the current task/process
void _exit(int code)
{
	int result = 0;
	SYSCALL1(SYSCALL_EXIT, result, code);
	while(1);
}

// Replace the current memory space with a new one
int execve(const char *name, char * const argv[], char * const env[])
{
	int result;
	SYSCALL3(SYSCALL_EXECVE, result, name, argv, env);
	if( result < 0 ){
		errno = -result;
		return -1;
	}
	return result;
}

// Duplicate the current memory space in a new process/task
int fork()
{
	int result;
	SYSCALL0(SYSCALL_FORK, result);
	if( result < 0 ){
		errno = -result;
		return -1;
	}
	return result;
}

// Retrieve the current process identifier
int getpid()
{
	int result;
	SYSCALL0(SYSCALL_GETPID, result);
	return result;
}

// **STUB** Send a signal to another process
int kill(int pid, int sig)
{
	errno = ESRCH;
	return -1;
}

// Wait on a status change of a child
pid_t wait(pid_t *status)
{
	return waitpid((pid_t)-1, status, 0);
}

// Wait on a status change of child, children, or process
pid_t waitpid(pid_t pid, int* status, int options)
{
	int result;
	SYSCALL3(SYSCALL_WAITPID, result, pid, status, options);
	if( result < 0 ){
		errno = -result;
		return -1;
	}
	return (pid_t)result;
}

// Detach the current process from the terminal
int detach(pid_t pid)
{
	int result = 0;
	SYSCALL1(SYSCALL_DETACH, result, pid);
	errno = -result;
	if( result < 0 ){
		return -1;
	}
	return 0;
}

// Create a new interprocess message
message_t* message_alloc( void )
{
	return (message_t*)malloc(MESG_MAX_LENGTH + sizeof(message_t));
}

// Free a message
int message_free(message_t* message)
{
	free(message);
	return 0;
}

// Retrieve a message sent to us
int message_pop(message_t* mesg, unsigned int type, unsigned int flags)
{
	int result = 0;
	SYSCALL3(SYSCALL_MESG_POP, result, mesg, type, flags);
	if( result < 0 ){
		errno = result;
		return -1;
	}
	errno = 0;
	return result;
}

// Send a message to another process
int message_send(pid_t where, unsigned int type, const char* data, size_t length)
{
	int result = 0;
	SYSCALL4(SYSCALL_MESG_SEND, result, where, type, data, length);
	errno = -result;
	if( result < 0 ){
		return -1;
	}
	return 0;
}