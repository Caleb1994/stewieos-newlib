#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <stdarg.h>
#include <errno.h>

// Open a file
int open(const char *name, int flags, ...)
{
	int result;
	mode_t mode;
	
	// Grab the mode argument if it is there
	if( flags & O_CREAT )
	{
		va_list va;
		va_start(va, flags);
		mode = va_arg(va, mode_t);
		va_end(va);
	}
	
	SYSCALL3(SYSCALL_OPEN, result, name, flags, mode);
	if( result < 0 ){
		errno = -result;
		return -1;
	}
	return result;
}

// Move the file pointer
off_t lseek(int file, off_t ptr, int dir)
{
	int result;
	SYSCALL3(SYSCALL_LSEEK, result, file, ptr, dir);
	if( result < 0 ){
		errno = -result;
		return (off_t)-1;
	}
	return (off_t)result;
}

// Read a block of data from a file
_READ_WRITE_RETURN_TYPE read(int file, void *ptr, size_t len)
{
	int result;
	SYSCALL3(SYSCALL_READ, result, file, ptr, len);
	if( result < 0 ){
		errno = -result;
		return (_READ_WRITE_RETURN_TYPE)-1;
	}
	return (_READ_WRITE_RETURN_TYPE)result;
}

// Write a block of data to a file
_READ_WRITE_RETURN_TYPE write(int file, const void *ptr, size_t len)
{
	int result;
	SYSCALL3(SYSCALL_WRITE, result, file, ptr, len);
	if( result < 0 ){
		errno = -result;
		return (_READ_WRITE_RETURN_TYPE)-1;
	}
	return (_READ_WRITE_RETURN_TYPE)result;
}

/* Close a file descriptor */
int close(int file)
{
	int result;
	SYSCALL1(SYSCALL_CLOSE, result, file);
	if( result < 0 ){
		errno = -result;
		return -1;
	}
	return result;
}

// Retrieve information about a file descriptor
int fstat(int file, struct stat *st)
{
	int result;
	SYSCALL2(SYSCALL_FSTAT, result, file, st);
	if( result < 0 ){
		errno = -result;
		return -1;
	}
	return result;
}

// Check if the file is a TTY
int isatty(int file)
{
	int result;
	SYSCALL1(SYSCALL_ISATTY, result, file);
	if( result < 0 ){
		errno = result;
		return 0;
	}
	return 1;
}

// **STUB** Link one file to another
int link(const char *old, const char *new)
{
	errno = ENOENT;
	return -1;
}

// **STUB** Get information on an unopened file
int stat(const char *file, struct stat *st)
{
	errno = ENOENT;
	return -1;
}

// **STUB** Remove an inode from the file system
int unlink(const char *name)
{
	errno = ENOENT;
	return -1;
}

// Change the current working directory 
int chdir(const char* path)
{
	int result = 0;
	SYSCALL1(SYSCALL_CHDIR, result, path);
	errno = -result;
	if( result < 0 ){
		return -1;
	}
	return result;
}

// Get the current working directory
char* getcwd(char* buffer, size_t length)
{
	int result = 0;
	SYSCALL2(SYSCALL_GETCWD, result, buffer, length);
	errno = -result;
	if( result < 0 ){
		return NULL;
	}
	return buffer;
}

// Create a new filesystem node
int mknod(const char* name, mode_t mode, dev_t dev)
{
	int result = 0;
	SYSCALL3(SYSCALL_MKNOD, result, name, mode, dev);
	errno = -result;
	if( result < 0 ){
		return -1;
	}
	return 0;
}

// Check the access rights on a given file
int access(const char* pathname, int mode)
{
	int result = 0;
	SYSCALL2(SYSCALL_ACCESS, result, pathname, mode);
	errno = -result;
	if( result < 0 ){
		return -1;
	}
	return 0;
}

// **STUB** Execute special file/device commands
int ioctl(int fd, int request, ...)
{
	return -EINVAL;
}