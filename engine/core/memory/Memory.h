#pragma once

#include "Defines.h"
#include "MemoryTag.h"

#include <cstddef>

namespace memory
{
constexpr u64 FE_DEFAULT_INITIAL_HEAP_SIZE = 16ULL * 1024 * 1024; // 16 MB
constexpr u64 FE_DEFAULT_MAX_HEAP_SIZE = 512ULL * 1024 * 1024;    // 512 MB

/// @brief Initialize the memory system with the specified initial and maximum heap sizes.
/// @param initialSize The initial size of the memory system in bytes. Defaults to
/// FE_DEFAULT_INITIAL_HEAP_SIZE.
/// @param maxSize Maximum size of the memory system in bytes. Defaults to FE_DEFAULT_MAX_HEAP_SIZE.
void Init(u64 initialSize = FE_DEFAULT_INITIAL_HEAP_SIZE, u64 maxSize = FE_DEFAULT_MAX_HEAP_SIZE);

/// @brief Shutdown the memory system and free all allocated memory.
void Shutdown();

/// @brief Allocate a block of memory with the specified size, alignment, and memory tag.
/// @param size The number of bytes to allocate.
/// @param alignment The required alignment in bytes.
/// @param tag The memory category for profiling.
/// @param file The name of the source file where the allocation was made.
/// @param line The line number of the source file where the allocation was made.
/// @return A pointer to the allocated memory block, or nullptr if the allocation failed.
void* Alloc(u64 size, u64 alignment, MemoryTag tag, const char* file = nullptr, i32 line = 0);

/// @brief Free a previously allocated memory block.
/// @param ptr The pointer to free.
void Free(void* ptr);

/// @brief Prints memory statistics to the console.
void PrintStats();

} // namespace memory

#define FE_ALLOC(size, tag) memory::Alloc(size, alignof(max_align_t), tag, __FILE__, __LINE__)

#define FE_FREE(ptr)        memory::Free(ptr)