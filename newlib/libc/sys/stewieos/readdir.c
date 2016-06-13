#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/dirent.h>
#include <sys/syscall.h>
#include <errno.h>
#include <stdio.h>

// The internal structure of DIR
struct _DIR
{
	int fd;					// File descriptor
	struct dirent dirent;	// dirent buffer
};

// Open a directory
DIR* opendir(const char* name)
{
	int fd = open(name, /*O_DIRECTORY |*/ O_RDONLY);
	
	if( fd < 0 ){
		return NULL;
	}
	
	DIR* dir = (DIR*)malloc(sizeof(DIR));
	if( !dir ){
		errno = -ENOMEM;
		return NULL;
	}
	dir->fd = fd;
	
	return dir;
}

// Close a directory
int closedir(DIR* dir)
{
	int result = 0;
	if( (result = close(dir->fd)) < 0 ){
		return result;
	}
	free(dir);
	return 0;
}

// Read a directory entry
struct dirent* readdir(DIR* dir)
{
	int result = 0;
	SYSCALL3(SYSCALL_READDIR, result, dir->fd, &dir->dirent, 1);
	errno = -result;
	if( result <= 0 ){
		return NULL;
	}
	
	return &dir->dirent;
}

// Get the current directory entry index
long telldir(DIR* dirp)
{
	return (long)lseek(dirp->fd, 0, SEEK_CUR);
}

// Seek to a directory entry index
void seekdir(DIR* dirp, long p)
{
	lseek(dirp->fd, p, SEEK_SET);
}
