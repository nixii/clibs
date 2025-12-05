
/*
 * HEADERS
*/
#ifndef FS_H
#define FS_H

// Check if a path exsits. Returns true(1) if it exists and false(0) if it doesn't.
int fs_exists(const char *path);

// Check if a path is a directory. Returns true(1) if it is a directory and false(0) if it isn't, or if it doesn't exist.
int fs_is_directory(const char *path);

// Check if a path is a file. Returns true(1) if it is a file and false(0) if it isn't, or if it doesn't exist.
int fs_is_file(const char *path);

// Create a directory safely. Returns an error code. If the directory already exists, it still returns 0, but you can check errno.
int fs_mkdir(const char *path);

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
#else
	#include <sys/stat.h>
	#include <sys/types.h>
	#include <unistd.h>
	#define __FS_MKDIR(dir) mkdir(dir, 0775)
#endif

// General include statements
#include <string.h>
#include <stdio.h>

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



#endif