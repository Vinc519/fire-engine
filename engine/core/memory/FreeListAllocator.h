#pragma once

#include "Defines.h"

#include "MemoryTag.h"

struct BlockHeader
{
	u64 size;
	BlockHeader* prev;
	BlockHeader* next;
	u32 sentinel;
	MemoryTag tag;
	b8 isFree;
#ifdef FE_DEBUG
	const char* file;
	i32 line;
#endif
};

class FreeListAllocator
{
  public:
	/// @brief Initialize the allocator with an initial and maximum size.
	/// @param initialSize The initial size of the heap in bytes (default: 16 MB), it will be aligned to the system
	/// page size.
	/// @param maxSize The maximal size that the heap can reach (default: 1 GB). It will be aligned to
	/// the system page size.
	void Init(u64 initialSize, u64 maxSize);

	/// @brief Shutdown the allocator, free all memory, and report any memory leaks.
	void Shutdown();

	/// @brief Allocate memory with debug information if compiled in debug mode.
	/// @param size The number of bytes to allocate.
	/// @param alignment The required alignement in bytes (default via macro: max_align_t).
	/// @param tag The memory category for profiling.
	/// @param file The name of the source file where the allocation was made.
	/// @param line The line number of the source file where the allocation was made.
	/// @return A pointer to the allocated memory if it succeeds, nullptr otherwise.
	void* Alloc(u64 size, u64 alignment, MemoryTag tag, const char* file = nullptr, i32 line = 0);

	/// @brief Free a previously allocated memory block.
	/// @param ptr Pointer to free.
	void Free(void* ptr);

	/// @brief Print usage statistics of the heap.
	void PrintStats() const;

  private:
	void* m_reservedBase = nullptr;
	u64 m_maxSize = 0;
	u64 m_committedSize = 0;

	u64 m_totalAllocated = 0;
	u64 m_totalFreed = 0;
	u64 m_allocationCount = 0;
	u64 m_freeCount = 0;
	u64 m_sentinelCorruptionCount = 0;
	u64 m_growCount = 0;
	u64 m_peakCommittedSize = 0;

	u64 m_numberOfFreeBlocks = 0;
	u64 m_sizeOfTheLargestFreeBlock = 0;

	// Memory usage per tag: [0] = number of allocations, [1] = size
	u64 m_allocatedPerTag[static_cast<u8>(MemoryTag::Max)][2] = {0};
	u64 m_freedPerTag[static_cast<u8>(MemoryTag::Max)][2] = {0};

	u64 m_peakPerTag[static_cast<u8>(MemoryTag::Max)] = {0};

	BlockHeader* m_firstBlock = nullptr;
	BlockHeader* m_lastBlock = nullptr;
	BlockHeader* m_lastFreed = nullptr;

	/// @brief Grow the heap if necessary.
	/// @param requiredSize Space required to allocate the requested block.
	/// @return TRUE if enough space is available or if the growth succeeded, FALSE otherwise.
	b8 GrowHeap(u64 requiredSize);

	/// @brief Finds the best free block to reuse (best-fit search).
	/// @param size Number of required bytes to allocate.
	/// @return A pointer of the BlockHeader of the best-fit free block, or nullptr if none found.
	BlockHeader* FindFreeBlock(u64 size);

	/// @brief Merges a free block with adjacent free blocks (coalescing).
	/// @param block Free block to coalesce with its neighbors.
	void CoalesceBlock(BlockHeader*& block);

	/// @brief Report memory leaks at shutdown if there is any.
	void ReportLeaks() const;
};