#include "Thread.h"

#include "core/debug/Assert.h"
#include "core/debug/Logger.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

struct ThreadHandle
{
	HANDLE thread;
};

b8 Thread_Create(ThreadHandle* handle, ThreadFunc func, void* arg, const char* name)
{
	FE_ASSERT_MSG(handle != nullptr, "Thread handle is null");
	FE_ASSERT_MSG(func != nullptr, "Thread function is null");
	FE_ASSERT_MSG(arg != nullptr, "Thread argument is null");
	
	handle->thread = CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)func, arg, 0, nullptr);
	if (handle->thread == nullptr)
		FE_LOG_ERROR("Failed to create thread (error: %lu)", GetLastError());

	return handle->thread != nullptr;
}

void Thread_Join(ThreadHandle* handle)
{
	FE_ASSERT_MSG(handle != nullptr, "Thread handle is null, please pass a valid thread handle to join the thread");
	WaitForSingleObject(handle->thread, INFINITE);
}

void Thread_Destroy(ThreadHandle* handle)
{
	if (handle->thread != nullptr)
	{
		CloseHandle(handle->thread);
		handle->thread = nullptr;
	}
	else
		FE_LOG_WARN(
		    "Thread handle is null, please pass a valid thread handle to destroy the thread");
}

u32 CPU_GetCoreCount()
{
	return (static_cast<u32>(GetActiveProcessorCount(ALL_PROCESSOR_GROUPS)));
}