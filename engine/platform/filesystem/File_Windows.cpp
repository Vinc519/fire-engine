#include "File.h"

#include "core/debug/Assert.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

typedef struct FileHandle
{
	HANDLE handle;
} FileHandle;

FileHandle* File_Open(const char* __restrict name, FileOpenMode openMode, FileAccessMode accessMode)
{
	FE_ASSERT_MSG(name, "One or more parameters are NULL");

	FileHandle *handle = new FileHandle;
	u32 flagsOpenMode = 0, flagsAccessMode = 0;
	switch (openMode)
	{
		case FileOpenMode::CREATE:
			flagsOpenMode = CREATE_NEW;
			break;
		
		case FileOpenMode::OPEN:
			flagsOpenMode = OPEN_EXISTING;
			break;
		
		case FileOpenMode::OPEN_OR_CREATE:
			flagsOpenMode = OPEN_ALWAYS;
			break;
		
		case FileOpenMode::TRUNCATE:
			flagsOpenMode = TRUNCATE_EXISTING;
			break;
		default:
			break;
	}

	if (!!(accessMode & FileAccessMode::READ))
		flagsAccessMode |= GENERIC_READ;
	if (!!(accessMode & FileAccessMode::WRITE))
		flagsAccessMode |= GENERIC_WRITE;
	if (!!(accessMode & FileAccessMode::APPEND))
		flagsAccessMode |= FILE_APPEND_DATA;

	handle->handle = CreateFile(name, flagsAccessMode, 0, nullptr, flagsOpenMode, FILE_ATTRIBUTE_NORMAL, nullptr);
	return handle;
}

void File_Close(FileHandle* handle)
{
	FE_ASSERT_MSG(handle, "FileHandle is NULL");
	// TODO: Get the return value
	CloseHandle(handle->handle);
	delete handle;
	handle = nullptr;
}

bool File_Delete(const char* path)
{
	FE_ASSERT_MSG(path, "File path is NULL");
	// TODO: A file cannot be closed until all the HANDLE to it are closed by CloseHandle.
	return DeleteFile((LPCSTR)path);
}

bool File_Exists(const char* __restrict path)
{
	FE_ASSERT_MSG(path, "File path is NULL");

	DWORD attributs = GetFileAttributesA(path);
	if (attributs == INVALID_FILE_ATTRIBUTES)
	{
		return FALSE;
	}
	return TRUE;
}

u64 File_GetSize(FileHandle* handle)
{
	FE_ASSERT_MSG(handle, "FileHandle is NULL");

	LARGE_INTEGER size = { 0 };
	int res = GetFileSizeEx(handle->handle, &size);
	
	return size.QuadPart;
}

u64 File_Write(FileHandle *handle, const char *buffer, u64 bufferSize)
{
	FE_ASSERT_MSG(handle && buffer, "One or more parameters are NULL");

	u64 bytesWritten = 0;
	i8 res = WriteFile(handle->handle, (LPCVOID)buffer, (DWORD)bufferSize, (LPDWORD)&bytesWritten,
	                   nullptr);
	FE_ASSERT_MSG(bytesWritten == bufferSize, "To write bytes and written bytes are not equal");
	return res;
}

u64 File_Read(FileHandle *handle, char *outBuffer, u64 bufferSize)
{
	FE_ASSERT_MSG(handle && outBuffer, "One or more parameters are NULL");

	u64 bytesRead = 0;
	i8 res = ReadFile(handle->handle, outBuffer, bufferSize, (LPDWORD)&bytesRead, nullptr);
	FE_ASSERT_MSG(bytesRead <= bufferSize, "Bytes to read and bytes read are not equal");
	return res;
}

void File_Flush(FileHandle* handle)
{
	FE_ASSERT_MSG(handle, "FileHandle is NULL");

	// TODO: Get the return value.
	FlushFileBuffers(handle->handle);
}

u64 File_Seek(FileHandle* handle, i64 offset, FileSeekOrigin origin)
{
	FE_ASSERT_MSG(handle, "FileHandle is NULL");

	DWORD moveMethod = 0;
	switch (origin)
	{
		case FileSeekOrigin::Begin:
			moveMethod = FILE_BEGIN;
			break;
		case FileSeekOrigin::Current:
			moveMethod = FILE_CURRENT;
			break;
		case FileSeekOrigin::End:
			moveMethod = FILE_END;
			break;
		default:
			moveMethod = FILE_BEGIN;
			break;
	}

	LARGE_INTEGER distanceToMove = {0};
	LARGE_INTEGER newPosition;
	distanceToMove.QuadPart = offset;

	i32 result = SetFilePointerEx(handle->handle, distanceToMove, &newPosition, moveMethod);
	FE_ASSERT_MSG(result, "SetFilePointerEx failed");

	return static_cast<u64>(newPosition.QuadPart);
}
