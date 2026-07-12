#pragma once

#include "Defines.h"

/// @brief Reserves a block of virtual memory.
/// @param size The size of the memory block to reserve.
/// @return A pointer to the reserved memory block, or nullptr if the reservation fails.
void* VirtualMemory_Reserve(u64 size);

/// @brief Commits a block of virtual memory, making it accessible for read and write operations.
/// @param ptr A pointer to the memory block to commit.
/// @param size The size of the memory block to commit.
/// @return True if the memory was successfully committed, false otherwise.
b8 VirtualMemory_Commit(void* ptr, u64 size);

/// @brief Decommits a block of virtual memory, making it inaccessible for read and write operations.
/// @param ptr A pointer to the memory block to commit.
/// @param size The size of the memory block to commit.
/// @return True if the memory was successfully committed, false otherwise.
b8 VirtualMemory_Decommit(void* ptr, u64 size);

/// @brief Releases a block of virtual memory, freeing the reserved memory.
/// @param ptr A pointer to the memory block to release.
/// @param size The size of the memory block to release.
void VirtualMemory_Release(void* ptr, u64 size);

/// @brief Gets the system's memory page size.
/// @return The size of a memory page in bytes.
u64 VirtualMemory_GetPageSize();