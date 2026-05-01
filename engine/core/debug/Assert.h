#pragma once

#include "Defines.h"

#ifdef FE_COMPILER_MSVC
#	include <intrin.h>
#	define FE_DEBUG_BREAK() __debugbreak()
#else
#	define FE_DEBUG_BREAK() __builtin_trap()
#endif

void FE_AssertReport(const char *expr, const char *msg, const char *file, i32 line);

#ifndef FE_RELEASE
#	define FE_ASSERT(expr)                                      \
		do                                                      \
		{                                                       \
			if (expr)                                           \
			{                                                   \
			}                                                   \
			else                                                \
			{                                                   \
				FE_AssertReport(#expr, "", __FILE__, __LINE__); \
				FE_DEBUG_BREAK();                               \
			}                                                   \
		} while (0)

#	define FE_ASSERT_MSG(expr, msg)                              \
		do                                                       \
		{                                                        \
			if (expr)                                            \
			{                                                    \
			}                                                    \
			else                                                 \
			{                                                    \
				FE_AssertReport(#expr, msg, __FILE__, __LINE__); \
				FE_DEBUG_BREAK();                                \
			}                                                    \
		} while (0)
#else
#	define FE_ASSERT(expr)          ((void)(expr))
#	define FE_ASSERT_MSG(expr, msg) ((void)(expr))
#endif
