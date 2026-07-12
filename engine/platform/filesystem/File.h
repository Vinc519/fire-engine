#pragma once

#include "Defines.h"
#include "Utils.h"

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
	TRUNCATE,
};

enum class FileAccessMode : u8
{
	READ = 0x1,
	WRITE = 0x2,
	APPEND = 0x4,
};

enum class FileSeekOrigin : u8
{
	Begin,
	Current,
	End,
};

// Operator macro
FE_DEFINE_ENUM_FLAGS(FileAccessMode, u8)


FileHandle* File_Open(const char *__restrict name, FileOpenMode openMode, 
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
u64 File_Write(FileHandle *handle, const char *buffer, u64 bufferSize);

// Reads up to size bytes from the file into outBuffer.
// Returns the number of bytes actually read.
u64 File_Read(FileHandle *handle, char *outBuffer, u64 bufferSize);

// Flushes any OS-buffered writes to disk.
// Always call this before File_Close() when write correctness matters (e.g. logger).
void File_Flush(FileHandle* handle);

// Moves the file pointer by offset bytes from origin.
// Returns the new absolute position in the file.
u64 File_Seek(FileHandle *handle, i64 offset, FileSeekOrigin origin);
