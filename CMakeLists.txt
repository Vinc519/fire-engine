cmake_minimum_required(VERSION 3.25)
project(FireEngine VERSION 0.1.0 LANGUAGES C CXX)

# ==============================================================================
# Global Settings
# ==============================================================================
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# ==============================================================================
# Platform Detection
# ==============================================================================
if(WIN32)
    set(FE_PLATFORM_WINDOWS ON)
    add_compile_definitions(FE_PLATFORM_WINDOWS)
elseif(UNIX AND NOT APPLE)
    set(FE_PLATFORM_LINUX ON)
    add_compile_definitions(FE_PLATFORM_LINUX)
else()
    message(FATAL_ERROR "Unsupported platform. Fire Engine supports Windows and Linux only.")
endif()

# ==============================================================================
# Build Options
# ==============================================================================
option(FE_BUILD_TESTS    "Build unit tests"         ON)
option(FE_BUILD_SANDBOX  "Build sandbox application" ON)
option(FE_ENABLE_VULKAN  "Enable Vulkan renderer"   ON)
option(FE_ENABLE_DX12    "Enable DirectX 12 renderer (Windows only)" OFF)
option(FE_ENABLE_OPENGL  "Enable OpenGL renderer"   OFF)

# DX12 is Windows-only
if(FE_ENABLE_DX12 AND NOT FE_PLATFORM_WINDOWS)
    message(WARNING "DirectX 12 is only available on Windows. Disabling FE_ENABLE_DX12.")
    set(FE_ENABLE_DX12 OFF)
endif()

# ==============================================================================
# Static Analysis (clang-tidy + clang-format)
# ==============================================================================
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake")
#include(StaticAnalysis) # clang-tidy + clang-format

# ==============================================================================
# Sanitizers
# ==============================================================================
include(Sanitizers) # ASan, LSan, UBSan
fe_apply_sanitizers()

# ==============================================================================
# Third Party
# ==============================================================================
add_subdirectory(third_party)

# ==============================================================================
# Engine
# ==============================================================================
add_subdirectory(engine)

# ==============================================================================
# Sandbox
# ==============================================================================
if(FE_BUILD_SANDBOX)
    add_subdirectory(sandbox)
endif()

# ==============================================================================
# Tests
# ==============================================================================
if(FE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()