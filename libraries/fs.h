
/*
 * HEADERS
*/
#ifndef FS_H
#define FS_H

// Resolve a path
char * fs_resolved_path(const char *path);

// Check if a path exsits. Returns true(1) if it exists and false(0) if it doesn't.
int fs_exists(const char *path);

// Check if a path is a directory. Returns true(1) if it is a directory and false(0) if it isn't, or if it doesn't exist.
int fs_is_directory(const char *path);

// Check if a path is a file. Returns true(1) if it is a file and false(0) if it isn't, or if it doesn't exist.
int fs_is_file(const char *path);

// Create a directory safely. Returns an error code. If the directory already exists, it still returns 0, but you can check errno.
int fs_mkdir(const char *path);

// Create every directory recursively. However, this does use a lot more memory (~1-4 KiB of RAM when being run). It is recommended to use fs_mkdir if that matters.
int fs_mkdir_all(const char *path);


// Platform-specific constants
#ifdef _WIN32
	
	#define __FS_MAX_PATH_BUFFER MAX_PATH
#else
	#define __FS_MAX_PATH_BUFFER PATH_MAX
#endif

#endif


/*
 * Implementation
*/

// Header guard
#ifdef FS_IMPLEMENTATION

// Platform-specific include statements
#ifdef _WIN32
	#include <windows.h>
	#include <direct.h>
	#define __FS_MKDIR(dir) _mkdir(dir)
	#define __FS_RESOLVEPATH()
#else
	#include <sys/syslimits.h>
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <unistd.h>
	#define __FS_MKDIR(dir) mkdir(dir, 0775)
#endif

// General include statements
#include <string.h>
#include <stdio.h>
#include <errno.h>

// Externals
extern int errno;

// Return whether or not a path exists
int fs_exists(const char *path) {
#ifdef _WIN32
	DWORD a = GetFileAttributesA(path);
	return (a != INVALID_FILE_ATTRIBUTES);
#else
	return access(path, F_OK) == 0;
#endif
}

// Return whether or not a path is both a directory and existent
int fs_is_directory(const char *path) {
#ifdef _WIN32
	DWORD a = GetFileAttributesA(path);
    	return a != INVALID_FILE_ATTRIBUTES && a & FILE_ATTRIBUTE_DIRECTORY;
#else
	struct stat st;
	return stat(path, &st) == 0 && S_ISDIR(st.st_mode);
#endif
}

// Return whether or not a path is both a file and existent
int fs_is_file(const char *path) {
#ifdef _WIN32
	DWORD a = GetFileAttributesA(path);
	return a != INVALID_FILE_ATTRIBUTES && !(a & FILE_ATTRIBUTE_DIRECTORY);
#else
	struct stat st;
	return stat(path, &st) == 0 && !S_ISDIR(st.st_mode);
#endif
}

// Create a directory
int fs_mkdir(const char *path) {
	int res = __FS_MKDIR(path);
	if (res != 0)
		if (res == EEXIST)
			return 0;
	return res;
}

// Recursively create directories
int fs_mkdir_all(const char *path) {
	char *npath = fs_resolved_path(path);
	char buf[__FS_MAX_PATH_BUFFER + 1];

	int idx = 0;
	while (npath[idx]) {
		if (npath[idx] == '/' || npath[idx] == '\\') {
			memcpy(&buf, npath, idx + 1);
			buf[idx+1] = '\0';
			int res = fs_mkdir(buf);
			printf("%s\n", buf);
			if (res != 0)
				return res;
		}
		++idx;
	}

	return 0;
}

#endif