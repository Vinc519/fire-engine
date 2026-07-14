#include <core/debug/Logger.h>
#include <platform/filesystem/File.h>
#include <platform/filesystem/Directory.h>
#include "core/memory/Memory.h"

#include <memory>

void MemoryTest()
{
	memory::Init(1 * 1024 * 1024,
	            512 * 1024 * 1024); // Initialize with 16 MB initial and 512 MB max

	void* a = FE_ALLOC(64, MemoryTag::Debug);
	FE_FREE(a);

	memory::Shutdown();
}

// Entry point for the Fire Engine sandbox.
int main()
{
	fe::Logger::Init("log.txt");

	FE_LOG_INFO("Hello World!");

	MemoryTest();
	// Files

	/*FileHandle* handle = FileOpen("blablabla.txt", FileOpenMode::CREATE, FileAccessMode::WRITE | FileAccessMode::READ);
	FE_ASSERT_MSG(handle != nullptr, "File handle created !");

	b8 isExists = FileExists("blablabla.txt");
	if (isExists)
		printf("File exists !\n");
	else
		printf("File doesn't exists !\n");

	FileWrite(handle, "My name is Jacob and I love programming in C.", 46);

	File_Seek(handle, 0, FileSeekOrigin::Begin);

	char buffer[128] = { 0 };
	FileRead(handle, buffer, 128);
	printf("Buffer read is: %s", buffer);

	FileClose(handle);

	FileDelete("blablabla.txt");*/


	// Directories

	/*int res = DirectoryExists("tests");
	printf("DirectoryExists is : %d\n", res);

	res = DirectoryCreate("tests");
	printf("DirectoryCreate is : %d\n", res);

	res = DirectoryExists("tests");
	printf("DirectoryExists is : %d\n", res);

	FileHandle *handle = FileOpen("./tests/blablabla.txt", FileOpenMode::CREATE,
	                              FileAccessMode::WRITE | FileAccessMode::READ);
	FileHandle *handle2 = FileOpen("./tests/blablabla2.txt", FileOpenMode::CREATE,
	                              FileAccessMode::WRITE | FileAccessMode::READ);

	FileWrite(handle, "My name is Jacob and I love programming in C.", 46);
	FileWrite(handle2, "My name is Jacob and I love programming in C 2.", 48);

	FileClose(handle);
	FileClose(handle2);

	constexpr u32 MAX = 64;
	char* names[MAX];
	char buffer[MAX * FE_FILESYSTEM_MAX_PATH];
	u32 count = DirectoryListFiles("tests", names, buffer, MAX);
	printf("Files list in 'tests' directory :");
	for (u32 i = 0; i < count; i++)
		printf("\n%s", names[i]);

	FileDelete("./tests/blablabla.txt");
	FileDelete("./tests/blablabla2.txt");

	DirectoryDelete("test", TRUE);*/

	/*res = DirectoryDelete("tests", FALSE);
	printf("DirectoryDelete is : %d\n", res);*/

	fe::Logger::Shutdown();
	return 0;
}
