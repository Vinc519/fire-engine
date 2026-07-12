#include "File.h"

#include "core/debug/Assert.h"

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

struct FileHandle
{
	i32 fileDescriptor;
};

void File_Open(FileHandle* handle, const char* __restrict name, FileOpenMode openMode, FileAccessMode accessMode)
{
	FE_ASSERT_MSG(handle != nullptr, "The specified handle is null");
	FE_ASSERT_MSG(name != nullptr, "The specified name is null");

	switch (openMode)
	{
		case FileOpenMode::CREATE:
			break;
		case FileOpenMode::OPEN:
			break;
		case FileOpenMode::OPEN_OR_CREATE:
			break;
		case FileOpenMode::TRUNCATE:
			break;
		default:
			break;
	}

	handle->fileDescriptor = open(name, )
}

void File_Close(FileHandle* handle)
{
	close(handle->fileDescriptor);
}

bool File_Delete(const char* path)
{
	
}

bool File_Exists(const char* path)
{
	access()
}

u64 File_GetSize(FileHandle* handle)
{
	struct stat fileStatistics = {0};
	i32 result = fstat(handle->fileDescriptor, &fileStatistics);

	return fileStatistics.st_size;
}

u64 File_Write(FileHandle* handle, const void* data, u64 size)
{
	write();
}

u64 File_Read(FileHandle* handle, void* outBuffer, u64 size)
{
	read();
}

void File_Flush(FileHandle* handle)
{
	
}

u64 File_Seek(FileHandle* handle, i64 offset, FileSeekOrigin origin)
{

}