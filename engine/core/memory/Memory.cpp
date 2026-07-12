#include "Memory.h"
#include "FreeListAllocator.h"

#include "core/debug/Logger.h"

#include <string>

namespace memory
{

static FreeListAllocator s_freelist;

void Init(u64 initialSize, u64 maxSize)
{
	s_freelist.Init(initialSize, maxSize);
}

void Shutdown()
{
	s_freelist.Shutdown();
}

void* Alloc(u64 size, u64 alignment, MemoryTag tag, const char* file, i32 line)
{
	if (size == 0 || tag == MemoryTag::Max)
	{
		FE_LOG_ERROR("Attempted to allocate memory with size 0 or invalid tag.");
		return nullptr;
	}

#ifdef FE_DEBUG
	if (file == nullptr || line == 0)
		FE_LOG_WARN("Attempted to allocate memory with invalid file or line number.");
#endif // FE_DEBUG

	file = strstr(file, "fire-engine") + 12;

	return s_freelist.Alloc(size, alignment, tag, file, line);
}

void Free(void* ptr)
{
	if (ptr == nullptr)
	{
		FE_LOG_WARN("Attempted to free a null pointer.");
		return;
	}
	s_freelist.Free(ptr);
}

void PrintStats()
{
	s_freelist.PrintStats();
}

} // namespace memory