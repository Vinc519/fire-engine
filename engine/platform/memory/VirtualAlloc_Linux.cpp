#include "VirtualAlloc.h"

#include "core/debug/Logger.h"

#include <errno.h>
#include <sys/mman.h>
#include <unistd.h>

void* VirtualMemory_Reserve(u64 size)
{
	void* result = mmap(nullptr, size, PROT_NONE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (result == MAP_FAILED)
	{
		FE_LOG_ERROR("mmap64 (reserve) failed with error code: %d", errno);
		return nullptr;
	}
	return result;
}

b8 VirtualMemory_Commit(void* ptr, u64 size)
{
	i32 result = mprotect(ptr, size, PROT_READ | PROT_WRITE);
	if (result != 0)
		FE_LOG_ERROR("mprotect (commit) failed with error code: %d", errno);
	return result == 0;
}

b8 VirtualMemory_Decommit(void* ptr, u64 size)
{
	i32 result = mprotect(ptr, size, PROT_NONE);
	if (result != 0)
	{
		FE_LOG_ERROR("mprotect (decommit) failed with error code: %d", errno);
		return FALSE;
	}

	// Advise the kernel that the pages are no longer needed, allowing it to free up physical
	// memory.
	result = madvise(ptr, size, MADV_DONTNEED);
	if (result != 0)
		FE_LOG_WARN("madvise (decommit) failed with error code: %d", errno);

	return TRUE;
}

void VirtualMemory_Release(void* ptr, u64 size)
{
	i32 result = munmap(ptr, size);
	if (result != 0)
		FE_LOG_ERROR("munmap failed with error code: %d", errno);
}

u64 VirtualMemory_GetPageSize()
{
	u64 result = sysconf(_SC_PAGESIZE);
	FE_ASSERT_MSG(result > 0, "sysconf(_SC_PAGESIZE) failed");
	return static_cast<u64>(result);
}
