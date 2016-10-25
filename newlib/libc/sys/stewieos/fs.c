#define _GNU_SOURCE
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <stdio.h>
#include <sys/syscall.h>
#include <stdarg.h>
#include <errno.h>
#include <sys/mntent.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

ssize_t getdelim(char** pLine, size_t* pLen, int delim, FILE* stream);
ssize_t getline(char** pLine, size_t* pLen, FILE* stream);

int dup(int fd)
{
	int result;
	SYSCALL2(SYSCALL_DUP2, result, fd, -1);
	if( result < 0 ){
		errno = -result;
		return -1;
	}
	return result;
}

int dup2(int fd, int otherfd)
{
	int result;
	SYSCALL2(SYSCALL_DUP2, result, fd, otherfd);
	if( result < 0 ){
		errno = -result;
		return -1;
	}
	return result;
}

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
	int fd = open(file, O_RDONLY);
	if( fd < 0 ){
		return -1;
	}
	int result = fstat(fd, st);
	close(fd);
	return result;
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

// Unlink (remove) a file
int unlink(const char* filename)
{
	int result = 0;
	SYSCALL1(SYSCALL_UNLINK, result, filename);
	if( result < 0 ){
		errno = -result;
		return -1;
	}
	return 0;
}

// Read a string until the end of the line ('\n')
ssize_t getline(char** pLine, size_t* pLen, FILE* stream)
{
	return getdelim(pLine, pLen, '\n', stream);
}

// Read a string until the given delimeter or EOF, whichever is first.
ssize_t getdelim(char** pLine, size_t* pLen, int delim, FILE* stream)
{
	// We need this outside the loop
	size_t idx = 0;
	int chr = 0;

	// Read characters until EOF or `delim`
	for( idx = 0; (chr = fgetc(stream)) != EOF; ++idx )
	{
		// increase the size of the buffer if needed
		if( *pLine == NULL || idx >= *pLen ){
			*pLen += 32; // we increment 32 bytes at a time
			*pLine = realloc(*pLine, *pLen);
		}
		(*pLine)[idx] = chr;
		if( chr == '\n' ) break;
	}

	// Add an extra byte if needed for null termination
	if( (idx+1) == *pLen ){
		*pLine = realloc(*pLine, *pLen + 1);
		*pLen += 1;
	}

	// Add null termination
	(*pLine)[idx+1] = 0;

	// If we an EOF, return -1
	if( feof(stream) ){
		return -1;
	} else {
		// Otherwise return the number of characters read
		return idx+1;
	}
}

FILE* setmntent(const char* filename, const char* mode)
{
	FILE* stream = fopen(filename, mode);
	return stream;
}

struct mntent* getmntent(FILE* stream)
{
	static struct mntent mnt;
	static char* line = NULL;
	static size_t len = 0;

	/* NOTE: I don't like this. The memory is never free'd.
			but I don't know of a good way without limiting
			line length.
	*/
	if( getline(&line, &len, stream) == -1 ){
		return NULL;
	}

	// Blindly parse... bad idea, but it works...
	mnt.mnt_fsname = strtok(line, " \t");
	mnt.mnt_dir = strtok(NULL, " \t");
	mnt.mnt_type = strtok(NULL, " \t");
	mnt.mnt_opts = strtok(NULL, " \t");
	mnt.mnt_freq = 0;
	mnt.mnt_passno = 0;
	
	return &mnt;
}

int addmntent(FILE* stream, const struct mntent* mnt)
{
	if( fprintf(stream, "%s\t%s\t%s\t%s\n", mnt->mnt_fsname, mnt->mnt_dir,
			mnt->mnt_type, mnt->mnt_opts) <= 0 ){
		return 1;
	}

	return 0;
}

int endmntent(FILE* stream)
{
	fclose(stream);
	return 1;
}

char* hasmntopt(const struct mntent* mnt, const char* opt)
{

	char* opts = strdup(mnt->mnt_opts);
	char* token = strtok(opts, ",");
	do
	{
		if( strcmp(token, opt) == 0 ){
			free(opts);
			return (token - opts) + mnt->mnt_opts;
		}
	} while( (token = strtok(NULL, ",")) != NULL );

	free(opts);

	return NULL;
}