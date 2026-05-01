#pragma once

#if defined(_MSC_VER)
#	define FE_COMPILER_MSVC
#elif defined(__clang__)
#	define FE_COMPILER_CLANG
#elif defined(__GNUC__) || defined(__gcc__)
#	define FE_COMPILER_GCC
#else
#	error "Fire Engine: unsupported compiler."
#endif

#if defined(FE_COMPILER_MSVC)
#	define STATIC_ASSERT static_assert
#else
#	define STATIC_ASSERT _Static_assert
#endif

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned long long u64;

typedef signed char i8;
typedef signed short i16;
typedef signed int i32;
typedef signed long long i64;

typedef float f32;
typedef double f64;

typedef char b8;
typedef int b32;

STATIC_ASSERT(sizeof(u8) == 1, "u8 must be 1 byte");
STATIC_ASSERT(sizeof(u16) == 2, "u16 must be 2 bytes");
STATIC_ASSERT(sizeof(u32) == 4, "u32 must be 4 bytes");
STATIC_ASSERT(sizeof(u64) == 8, "u64 must be 8 bytes");

STATIC_ASSERT(sizeof(i8) == 1, "i8 must be 1 byte");
STATIC_ASSERT(sizeof(i16) == 2, "i16 must be 2 bytes");
STATIC_ASSERT(sizeof(i32) == 4, "i32 must be 4 bytes");
STATIC_ASSERT(sizeof(i64) == 8, "i64 must be 8 bytes");

STATIC_ASSERT(sizeof(f32) == 4, "f32 must be 4 bytes");
STATIC_ASSERT(sizeof(f64) == 8, "f64 must be 8 bytes");

STATIC_ASSERT(sizeof(b8) == 1, "b8 must be 1 byte");
STATIC_ASSERT(sizeof(b32) == 4, "b32 must be 4 bytes");

#define TRUE 1
#define FALSE 0

#ifdef _MSC_VER
#	define INLINE inline
#	define FORCE_INLINE __forceinline
#else
#	define INLINE static inline
#	define FORCE_INLINE __attribute__((always_inline))
#endif

#if !defined(FE_PLATFORM_WINDOWS) && !defined(FE_PLATFORM_LINUX)
#	if defined(_WIN32)
#		define FE_PLATFORM_WINDOWS
#	elif defined(__linux__)
#		define FE_PLATFORM_LINUX
#	else
#		error "Fire Engine: unsupported platform."
#	endif
#endif

#if defined(NDEBUG)
#	define FE_RELEASE
#else
#	define FE_DEBUG
#endif

#if defined(FE_PLATFORM_WINDOWS)
#	define FE_EXPORT __declspec(dllexport)
#	define FE_IMPORT __declspec(dllimport)
#elif defined(FE_PLATFORM_LINUX)
#	define FE_EXPORT __attribute__((visibility("default")))
#	define FE_IMPORT
#endif
