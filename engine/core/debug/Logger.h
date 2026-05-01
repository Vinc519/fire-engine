// =============================================================================
// Logger.h
// =============================================================================
// Description : Logger header. There is only one instance of it for the 
//				 hole program. It has to be used via the macros.
//
// Language    : C++20
// Module      : engine/core/debug/
// Author      : Vinc519
// License     : MIT — Copyright (c) 2026 Vinc519
//               Full text available in LICENSE at the project root.
// =============================================================================

#pragma once

#include "Assert.h"

#define FE_LOG_WARNING_ENABLED 1
#define FE_LOG_INFO_ENABLED 1

#ifdef FE_RELEASE
#	define FE_LOG_DEBUG_ENABLED 0
#	define FE_LOG_TRACE_ENABLED 0
#else
#	define FE_LOG_DEBUG_ENABLED 1
#	define FE_LOG_TRACE_ENABLED 1
#endif

struct FILE;

namespace fe
{

enum class LogLevel : u8;
struct LogEntry;

class Logger
{
	public:
		/// @brief Initialize the Logger.
		/// @param[in] filename The name of the file where the logs should be saved.
		static void Init(const char* filename);

		/// @brief Shutdown the logger.
		static void Shutdown();

        /// @brief Indicates wether or not the logger is initialized.
        /// @return true if the logger is initialized, false otherwise.
        static b8 IsInitialized();

		/// @brief Core output function called by every FE_LOG_* macro. 
		///	       Safe to call before Init() — falls back to stderr.
		/// @param[in] level The logging level.
		/// @param[in] file The name of the file where the macro where called.
		/// @param[in] line The line of the file where the macro where called.
		/// @param[in] message The log message.
		/// @param[in] params Different values to put into the message.
		static void Output(LogLevel level, const char* file, i32 line, const char* message, ...);

	private:
	    /// @brief Format the message with the fields inside the structure.
	    /// @param[in] entry The log structure.
	    static void FormatMessage(LogEntry& entry, b8 useColor);
		
	    /// @brief Writes a fully formatted entry to stderr and, if specified, to the log file.
	    /// @param[in] entry The log structure.
	    static void Write(LogEntry& entry);

	    static FILE* s_file;
	    static b8 s_initialized;
};

} // namespace fe



#define FE_LOG_FATAL(message, ...)                                                          \
	do																						\
	{																						\
		fe::Logger::Output(fe::LogLevel::Fatal, __FILE__, __LINE__, message, ##__VA_ARGS__);\
	} while (0)

#define FE_LOG_ERROR(message, ...)                                                          \
	do																						\
	{																						\
		fe::Logger::Output(fe::LogLevel::Error, __FILE__, __LINE__, message, ##__VA_ARGS__);\
	} while (0)

#if FE_LOG_WARNING_ENABLED
#	define FE_LOG_WARNING(message, ...)																\
		do																							\
		{																							\
			fe::Logger::Output(fe::LogLevel::Warning, __FILE__, __LINE__, message, ##__VA_ARGS__);  \
		} while (0)
#else
#	define FE_LOG_WARNING(message, ...) do {} while (0)
#endif

#if FE_LOG_INFO_ENABLED
#	define FE_LOG_INFO(message, ...)															\
		do																						\
		{																						\
			fe::Logger::Output(fe::LogLevel::Info, __FILE__, __LINE__, message, ##__VA_ARGS__); \
		} while (0)
#else
#	define FE_LOG_INFO(message, ...) do {} while (0)
#endif

#if FE_LOG_DEBUG_ENABLED
#	define FE_LOG_DEBUG(message, ...)															\
		do																						\
		{																						\
			fe::Logger::Output(fe::LogLevel::Debug, __FILE__, __LINE__, message, ##__VA_ARGS__);\
		} while (0)
#else
#	define FE_LOG_DEBUG(message, ...) do {} while (0)
#endif

#if FE_LOG_TRACE_ENABLED
#	define FE_LOG_TRACE(message, ...)															\
		do																						\
		{																						\
			fe::Logger::Output(fe::LogLevel::Trace, __FILE__, __LINE__, message, ##__VA_ARGS__);\
		} while (0)
#else
#	define FE_LOG_TRACE(message, ...) do {} while (0)
#endif
