#pragma once

#include "Defines.h"

constexpr u16 MAX_WINDOWS_FILE_NAME_SIZE = 260;

constexpr u16 FE_FILESYSTEM_MAX_PATH = 512;

// Creates the directory at path.
// Returns true on success or if the directory already exists.
b8 DirectoryCreate(const char *path);

// Deletes the directory at path.
// If recursive is true, deletes all contents first.
// Returns true on success.
b8 DirectoryDelete(const char *path, b8 recursive);

// Returns true if a directory exists at the given path.
b8 DirectoryExists(const char *path);

// Fills outNames with pointers to null-terminated filenames found in path.
// outNames must point to an array of at least maxCount char* entries.
// outBuffer is a flat memory block used to store the filename strings —
//   its size should be at least maxCount * FE_FILESYSTEM_MAX_PATH bytes.
// Returns the number of files written into outNames (capped at maxCount).
//
// TODO: allocate make one call to get the number of files to allocate memory, and a second to get the names
u32 DirectoryListFiles(const char *path, char **outNames, char *outBuffer, u32 maxCount);