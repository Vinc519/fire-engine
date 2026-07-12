#pragma once

#include "Defines.h"

struct ThreadHandle;

// Signature every thread entry point must match.
// arg is a user-supplied opaque pointer, forwarded as-is.
typedef void (*ThreadFunc)(void* arg);

/// @brief Creates a new thread.
/// @param handle Pointer to the thread handle.
/// @param func Function to be executed by the thread.
/// @param arg Argument to be passed to the thread function.
/// @return True if the thread was created successfully, false otherwise.
b8 Thread_Create(ThreadHandle* handle, ThreadFunc func, void* arg, const char* name = nullptr);

/// @brief Waits for a thread to finish execution.
/// @param handle Pointer to the thread handle.
void Thread_Join(ThreadHandle* handle);

/// @brief Destroys a thread.
/// @param handle Pointer to the thread handle.
void Thread_Destroy(ThreadHandle* handle);

/// @brief Gets the number of CPU cores available on the system.
/// @return The number of CPU cores.
u32 CPU_GetCoreCount();