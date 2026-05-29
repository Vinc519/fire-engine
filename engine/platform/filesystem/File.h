#pragma once

#include "Defines.h"

struct FileHandle;

enum class FileOpenMode : u8
{
	// Create the file but don't open it.
	CREATE,
	// Open a file but don't create it.
	OPEN,
	// If the file exists, it opens it, otherwise, created it and then open it.
	OPEN_OR_CREATE,
	// Open the file and delete all the data inside.
	TRUNCATE
};

enum class FileAccessMode : u8
{
	READ = 0x1,
	WRITE = 0x2,
	APPEND = 0x4
};

// Files

void File_Open(FileHandle* handle, const char* __restrict name, FileOpenMode openMode,
                               FileAccessMode accessMode);

// Closes an open file handle.
// Calling File_Close on an already-closed handle is a no-op.
void File_Close(FileHandle* handle);

// Deletes the file at the given path.
// Returns true on success, false if the file does not exist or cannot be deleted.
bool File_Delete(const char* path);

// Returns true if a file exists at the given path.
bool File_Exists(const char* path);

// Returns the size of an open file in bytes.
// Returns 0 if the handle is invalid.
u64 File_GetSize(FileHandle* handle);

// Writes size bytes from data into the file at the current position.
// Returns the number of bytes actually written.
u64 File_Write(FileHandle* handle, const void* data, u64 size);

// Reads up to size bytes from the file into outBuffer.
// Returns the number of bytes actually read.
u64 File_Read(FileHandle* handle, void* outBuffer, u64 size);

// Flushes any OS-buffered writes to disk.
// Always call this before File_Close() when write correctness matters (e.g. logger).
void File_Flush(FileHandle* handle);


// Directories
// Creates the directory at path.
// Returns true on success or if the directory already exists.
bool Directory_Create(const char* path);

// Deletes the directory at path.
// If recursive is true, deletes all contents first.
// Returns true on success.
bool Directory_Delete(const char* path, bool recursive);

// Returns true if a directory exists at the given path.
bool Directory_Exists(const char* path);

// Fills outNames with pointers to null-terminated filenames found in path.
// outNames must point to an array of at least maxCount char* entries.
// outBuffer is a flat memory block used to store the filename strings —
//   its size should be at least maxCount * FE_FILESYSTEM_MAX_PATH bytes.
// Returns the number of files written into outNames (capped at maxCount).
//
// Example:
//   constexpr u32 MAX = 64;
//   const char*   names[MAX];
//   char          buffer[MAX * FE_FILESYSTEM_MAX_PATH];
//   u32 count = Directory_ListFiles("assets/", names, buffer, MAX);
//   for (u32 i = 0; i < count; i++)
//       FE_LOG_INFO(names[i]);
u32 Directory_ListFiles(const char* path, const char** outNames, char* outBuffer, u32 maxCount);
