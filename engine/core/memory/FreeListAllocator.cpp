#include "FreeListAllocator.h"

#include "core/debug/Assert.h"
#include "core/debug/Logger.h"
#include "platform/memory/VirtualAlloc.h"

constexpr u32 FE_ALLOC_SENTINEL = 0xDEADC0DE;

static constexpr u64 GetHeaderSize()
{
	return sizeof(BlockHeader);
}

constexpr u64 AlignUp(u64 size, u64 alignment)
{
	return (size + alignment - 1) & ~(alignment - 1);
}

void FreeListAllocator::Init(u64 initialSize, u64 maxSize)
{
	FE_ASSERT_MSG(initialSize > 0, "FreeListAllocator initial size must be > 0");
	FE_ASSERT_MSG(maxSize >= initialSize, "FreeListAllocator max size must be >= initial size");

	u64 pageSize = VirtualMemory_GetPageSize();
	u64 alignedInitialSize = AlignUp(initialSize, pageSize);
	u64 alignedMaxSize = AlignUp(maxSize, pageSize);

	m_reservedBase = VirtualMemory_Reserve(alignedMaxSize);
	if (m_reservedBase == nullptr)
	{
		FE_LOG_FATAL("FreeListAllocator failed to reserve %llu MB of virtual memory",
		             alignedMaxSize / (1024 * 1024));
		return;
	}

	m_maxSize = alignedMaxSize;

	if (!VirtualMemory_Commit(m_reservedBase, alignedInitialSize))
	{
		FE_LOG_ERROR("FreeListAllocator failed to commit initial %llu MB",
		             alignedInitialSize / (1024 * 1024));
		return;
	}
	m_committedSize = alignedInitialSize;

	m_firstBlock = reinterpret_cast<BlockHeader*>(m_reservedBase);
	m_firstBlock->tag = MemoryTag::Unknown;
	m_firstBlock->isFree = TRUE;
	m_firstBlock->sentinel = 0;
	m_firstBlock->size = alignedInitialSize;
	m_firstBlock->prev = nullptr;
	m_firstBlock->next = nullptr;
#ifdef FE_DEBUG
	m_firstBlock->file = nullptr;
	m_firstBlock->line = 0;
#endif

	m_lastBlock = m_firstBlock;

	FE_LOG_INFO(
	    "FreeListAllocator initialized with %llu MB initial (aligned), max %llu MB (aligned)",
	    alignedInitialSize / (1024 * 1024), alignedMaxSize / (1024 * 1024));
}

void FreeListAllocator::Shutdown()
{
	if (m_reservedBase == nullptr)
	{
		FE_LOG_ERROR("FreeListAllocator shutdown called without initialization.");
		return;
	}

	ReportLeaks();

	VirtualMemory_Release(m_reservedBase, m_maxSize);
	m_reservedBase = nullptr;
	m_maxSize = 0;
	m_committedSize = 0;

	FE_LOG_INFO("FreeListAllocator shutdown complete.");
}

b8 FreeListAllocator::GrowHeap(u64 requiredSize)
{
	u64 pageSize = VirtualMemory_GetPageSize();

	u64 commitSize = AlignUp(requiredSize, pageSize);

	if (m_committedSize + commitSize > m_maxSize)
	{
		FE_LOG_ERROR("FreeListAllocator cannot grow heap: requested %llu bytes, max is %llu bytes",
		             m_committedSize + commitSize, m_maxSize);
		return FALSE;
	}

	void* commitStart = reinterpret_cast<u8*>(m_reservedBase) + m_committedSize;
	if (!VirtualMemory_Commit(commitStart, commitSize))
	{
		FE_LOG_ERROR("FreeListAllocator failed to commit additional %llu bytes", commitSize);
		return FALSE;
	}

	m_committedSize += commitSize;
	m_growCount++;
	return TRUE;
}

BlockHeader* FreeListAllocator::FindFreeBlock(u64 size)
{
	BlockHeader *current = m_firstBlock, *bestFit = nullptr;
	u64 bestLostSize = 0;

	// Best-fit search: find the smallest free block that fits the requested size.
	while (current != nullptr)
	{
		if (current->isFree && current->size >= size)
		{
			u64 lostSize = current->size - size;
			if (bestFit == nullptr || lostSize < bestLostSize)
			{
				bestFit = current;
				bestLostSize = lostSize;

				// Perfect fit found, no need to continue searching.
				if (lostSize == 0)
					break;
			}
		}
		current = current->next;
	}

	return bestFit;
}

void FreeListAllocator::CoalesceBlock(BlockHeader*& block)
{
	if (block->prev != nullptr && block->prev->isFree == TRUE)
	{
		block->prev->size += block->size;
		block->prev->next = block->next;
		if (block->next != nullptr)
			block->next->prev = block->prev;
		if (m_lastBlock == block)
			m_lastBlock = block->prev;
		block = block->prev;
	}

	if (block->next != nullptr && block->next->isFree == TRUE)
	{
		BlockHeader* nextBlock = block->next;
		block->size += nextBlock->size;
		block->next = nextBlock->next;
		if (nextBlock->next != nullptr)
			nextBlock->next->prev = block;
		if (m_lastBlock == nextBlock)
			m_lastBlock = block;
	}
}

void FreeListAllocator::ReportLeaks() const
{
	u64 leakCount = m_allocationCount - m_freeCount;

	if (leakCount <= 0)
	{
		FE_LOG_INFO("FreeListAllocator reported no memory leaks.");
		return;
	}

	FE_LOG_WARN("FreeListAllocator reported %llu active allocation(s)", leakCount);

	u64 totalLeakedBytes = 0;
	BlockHeader* current = m_firstBlock;
	while (current != nullptr)
	{
		if (current->isFree == FALSE)
		{
			u64 userSize = current->size - GetHeaderSize();
			totalLeakedBytes += userSize;
#ifdef FE_DEBUG
			FE_LOG_WARN("	[LEAK] %llu bytes | Tag: %s | %s:%d", userSize,
			            MemoryTagToString(current->tag),
			            current->file != nullptr ? current->file : "unknown", current->line);
#else
			FE_LOG_WARN("	[LEAK] %llu bytes | Tag: %-10s", userSize,
			            MemoryTagToString(current->tag));
#endif // FE_DEBUG
		}
		current = current->next;
	}

	FE_LOG_WARN("FreeListAllocator reported %llu active allocation(s) and %llu bytes allocated",
	            leakCount, totalLeakedBytes);
	return;
}

void* FreeListAllocator::Alloc(u64 size, u64 alignment, MemoryTag tag, const char* file, i32 line)
{
	FE_ASSERT_MSG(size > 0, "FreeListAllocator cannot allocate less or equal than 0 bytes");
	FE_ASSERT_MSG(alignment <= 16, "FreeListAllocator cannot satisfy alignment > 16");

	BlockHeader* header = nullptr;
	u64 requiredSize = size + GetHeaderSize();

	if (m_lastFreed != nullptr && m_lastFreed->isFree == TRUE && m_lastFreed->size >= requiredSize)
	{
		header = m_lastFreed;
		m_lastFreed = nullptr;
	}
	else
		header = FindFreeBlock(requiredSize);

	if (header == nullptr)
	{
		if (!GrowHeap(requiredSize))
		{
			FE_LOG_ERROR("FreeListAllocator failed to grow heap for allocation of %llu bytes",
			             size);
			return nullptr;
		}

		BlockHeader* newHeader =
		    reinterpret_cast<BlockHeader*>(reinterpret_cast<u8*>(m_lastBlock) + m_lastBlock->size);
		header = newHeader;
		header->size = requiredSize;
		header->prev = m_lastBlock;
		header->next = nullptr;
		m_lastBlock->next = header;
		m_lastBlock = header;
	}
	else
	{
		if ((header->size - requiredSize) >= (GetHeaderSize() + 16))
		{
			BlockHeader* newHeader =
			    reinterpret_cast<BlockHeader*>(reinterpret_cast<u8*>(header) + requiredSize);
			newHeader->tag = MemoryTag::Unknown;
			newHeader->isFree = TRUE;
			newHeader->sentinel = 0;
			newHeader->size = header->size - requiredSize;
			newHeader->prev = header;
			newHeader->next = header->next;
			if (header->next != nullptr)
				header->next->prev = newHeader;
			header->next = newHeader;
			header->size = requiredSize;
		}
	}

	// Fill in the header
	header->tag = tag;
	header->isFree = FALSE;
	header->sentinel = FE_ALLOC_SENTINEL;
#ifdef FE_DEBUG
	header->file = file;
	header->line = line;
#endif // FE_DEBUG

	// Update stats
	m_totalAllocated += header->size;
	m_allocationCount++;
	m_allocatedPerTag[static_cast<u8>(tag)][0]++;
	m_allocatedPerTag[static_cast<u8>(tag)][1] += header->size;

	return reinterpret_cast<u8*>(header) + GetHeaderSize();
}

void FreeListAllocator::Free(void* ptr)
{
	if (ptr == nullptr)
		return;

	BlockHeader* header =
	    reinterpret_cast<BlockHeader*>(reinterpret_cast<u8*>(ptr) - GetHeaderSize());

	if (header->sentinel != FE_ALLOC_SENTINEL)
	{
		FE_LOG_ERROR("[DOUBLE FREE] %p - sentinel mismatch (expected 0x%X, found 0x%X)", ptr,
		             FE_ALLOC_SENTINEL, header->sentinel);
		m_sentinelCorruptionCount++;
		return;
	}

	header->isFree = TRUE;
	header->sentinel = 0;

	m_totalFreed += header->size;
	m_freeCount++;
	m_freedPerTag[static_cast<u8>(header->tag)][0]++;
	m_freedPerTag[static_cast<u8>(header->tag)][1] += header->size;

	CoalesceBlock(header);

	m_lastFreed = header;
}

void FreeListAllocator::PrintStats() const
{
	FE_LOG_INFO("FreeListAllocator Stats:");
	FE_LOG_INFO("	Total Allocated: %llu bytes", m_totalAllocated);
	FE_LOG_INFO("	Total Freed: %llu bytes", m_totalFreed);
	FE_LOG_INFO("	Allocation Count: %llu", m_allocationCount);
	FE_LOG_INFO("	Free Count: %llu", m_freeCount);
	FE_LOG_INFO("	Sentinel Corruption Count: %llu", m_sentinelCorruptionCount);
	FE_LOG_INFO("	Grow Count: %llu", m_growCount);
	FE_LOG_INFO("	Peak Committed Size: %llu bytes", m_peakCommittedSize);
	for (u8 i = 0; i < static_cast<u8>(MemoryTag::Max); ++i)
	{
		const char* tagName = MemoryTagToString(static_cast<MemoryTag>(i));
		FE_LOG_INFO("		Tag: %s, Allocated: %llu bytes in %llu allocations, Freed: %llu bytes "
		            "in %llu frees",
		            tagName, m_allocatedPerTag[i][1], m_allocatedPerTag[i][0], m_freedPerTag[i][1],
		            m_freedPerTag[i][0]);
	}
}
