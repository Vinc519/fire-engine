// =============================================================================
// Logger.cpp
// =============================================================================
// Description : Logger implementation. There is only one instance of it for the
//				 hole program. It has to be used via the macros.
//
// Language    : C++20
// Module      : engine/core/debug/
// Author      : Vinc519
// License     : MIT — Copyright (c) 2026 Vinc519
//               Full text available in LICENSE at the project root.
// =============================================================================

#include "Logger.h"
#include "ANSI-color-codes.h"

#include <cstdarg>
#include <cstdio>
#include <ctime>
#include <string>

namespace fe
{

constexpr i16 LOG_MESSAGE_MAX_LENGTH = 1024;

/// @brief Structure that represents all the information about a log.
struct LogEntry
{
	LogLevel level;
	const char *file;
	i32 line;
	i64 timestamp;
	char message[LOG_MESSAGE_MAX_LENGTH];
};

FILE *Logger::s_file = nullptr;
b8 Logger::s_initialized = false;

void Logger::Init(const char *filename)
{
	i32 check = 0;
	if (filename != nullptr)
	{
#ifdef FE_PLATFORM_WINDOWS
		// Safe MSVC variant of fopen.
		errno_t err = fopen_s(&s_file, filename, "a");
		if (err != 0 || s_file == nullptr)
		{
			check = fprintf(stderr, "[Logger] Failed to open log file: %s\n", filename);
			FE_ASSERT_MSG(check > 0, "Failed to print into the console");
			s_file = nullptr;
		}
#else
		s_file = fopen(filename, "a");
		if (s_file == nullptr)
		{
			check = fprintf(stderr, "[Logger] Failed to open log file: %s\n", filename);
			FE_ASSERT_MSG(check > 0, "Failed to print into the console");
		}
#endif
	}

	s_initialized = true;
}

void Logger::Shutdown()
{
	i32 check = 0;
	if (s_file != nullptr)
	{
		check = fflush(s_file);
		FE_ASSERT_MSG(check == 0, "Cannot flush the buffer into the file");

		check = fclose(s_file);
		FE_ASSERT_MSG(check == 0, "Cannot close the file");

		s_file = nullptr;
	}

	s_initialized = false;
}

b8 Logger::IsInitialized() { return s_initialized; }

void Logger::FormatMessage(LogEntry &entry, char *buffer, b8 useColor)
{
	// Resolve time — use the thread-safe variant on each platform.
	struct tm timeInfo = {};
	i32 check = 0;
	const char *colorBegin = BWHT, *label = nullptr;

	time_t timestamp = static_cast<time_t>(entry.timestamp);
#ifdef FE_PLATFORM_WINDOWS
	FE_ASSERT_MSG(localtime_s(&timeInfo, &timestamp) == 0, "Error while getting time");
#else
	FE_ASSERT_MSG(localtime_r(&timestamp, &timeInfo) != nullptr, "Error while getting time");
#endif

	switch (entry.level)
	{
		case LogLevel::Fatal:
			colorBegin = BMAG;
			label = "FATAL";
			break;
		case LogLevel::Error:
			colorBegin = BRED;
			label = "ERROR";
			break;
		case LogLevel::Warning:
			colorBegin = BYEL;
			label = "WARN";
			break;
		case LogLevel::Info:
			colorBegin = BCYN;
			label = "INFO";
			break;
		case LogLevel::Debug:
			colorBegin = BLU;
			label = "DEBUG";
			break;
		case LogLevel::Trace:
			colorBegin = WHT;
			label = "TRACE";
			break;
		default:
			colorBegin = BMAG;
			label = "FATAL";
			break;
	}

	if (useColor)
		check = snprintf(buffer, LOG_MESSAGE_MAX_LENGTH,
		                 "[%02d/%02d/%04d %02d:%02d:%02d] [%s%s%s] %s:%d - %s\n", timeInfo.tm_mday,
		                 timeInfo.tm_mon + 1, 1900 + timeInfo.tm_year, timeInfo.tm_hour,
		                 timeInfo.tm_min, timeInfo.tm_sec, colorBegin, label, COLOR_RESET,
		                 entry.file, entry.line, entry.message);
	else
		check = snprintf(
		    buffer, LOG_MESSAGE_MAX_LENGTH, "[%02d/%02d/%04d %02d:%02d:%02d] [%s] %s:%d - %s\n",
		    timeInfo.tm_mday, timeInfo.tm_mon + 1, 1900 + timeInfo.tm_year, timeInfo.tm_hour,
		    timeInfo.tm_min, timeInfo.tm_sec, label, entry.file, entry.line, entry.message);
	FE_ASSERT_MSG(check >= 0 && check <= LOG_MESSAGE_MAX_LENGTH,
	              "Buffer overflow on entry.message");
}

void Logger::Write(LogEntry &entry)
{
	int check = 0;
	char buffer[LOG_MESSAGE_MAX_LENGTH] = {0};

	entry.file = strstr(entry.file, "fire-engine") + 12;

	FormatMessage(entry, buffer, TRUE);
	check = fprintf(stderr, "%s", buffer);
	FE_ASSERT_MSG(check > 0 && check <= LOG_MESSAGE_MAX_LENGTH, "Buffer overflow on entry.message");

	// Write to file without ANSI codes if a file is open.
	if (s_file != nullptr)
	{
		FormatMessage(entry, buffer, FALSE);
		check = fputs(buffer, s_file);
		FE_ASSERT_MSG(check >= 0, "The log message wasn't written in the file");
		check = fflush(s_file);
		FE_ASSERT_MSG(check == 0, "Cannot flush the buffer into the file");
	}
}

void Logger::Output(LogLevel level, const char *file, i32 line, const char *message, ...)
{
	LogEntry entry = {
	    .level = level,
		.file = file,
		.line = line,
		.timestamp = time(nullptr),
		.message = { 0 }
	};

	// Format the caller's message into entry.message first.
	va_list args;
	va_start(args, message);
	int check = vsnprintf(entry.message, LOG_MESSAGE_MAX_LENGTH, message, args);
	FE_ASSERT_MSG(check >= 0 && check < LOG_MESSAGE_MAX_LENGTH,
	              "Buffer overflow on entry.message");
	va_end(args);

	// Then assemble the full output line into entry.formatted.
	Write(entry);
}

} // namespace fe
