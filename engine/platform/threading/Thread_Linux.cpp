#include "Thread.h"

#include <unistd.h>
#include <pthread.h>

struct ThreadHandle
{
	pthread_t thread;
};

b8 Thread_Create(ThreadHandle* handle, void (*func)(void*), void* __restrict arg)
{
	pthread_create(&handle->thread, nullptr, func, arg);
}

void Thread_Join(ThreadHandle* handle)
{
	pthread_join(handle->thread, nullptr);
}

void Thread_Destroy(ThreadHandle* handle)
{
	
}

u32 CPU_GetCoreCount()
{
	long count = sysconf(_SC_NPROCESSORS_ONLN);
	return count > 0 ? static_cast<u32>(count) : 1;
}
