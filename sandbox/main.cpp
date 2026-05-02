#include <core/debug/Logger.h>

#include <cstdio>
// Entry point for the Fire Engine sandbox.

int main()
{
	fe::Logger::Init("log.txt");

	printf("Fire Engine - Sandbox\n");
	FE_LOG_INFO("Print message :%d", 8);

	fe::Logger::Shutdown();
	return 0;
}
