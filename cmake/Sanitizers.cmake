# =============================================================================
# cmake/Sanitizers.cmake — Fire Engine
# Configuration of debugging tools for Debug builds
# =============================================================================
#
# This module configures sanitizers (dynamic analysis tools) to detect
# memory errors and undefined behaviors during code execution.
# Configuration varies based on the platform and compiler used.
#
# =============================================================================

option(FE_ENABLE_ASAN  "Enable AddressSanitizer + LeakSanitizer (Debug only)" OFF)
option(FE_ENABLE_UBSAN "Enable UndefinedBehaviorSanitizer (for GCC/Clang, Debug only)" OFF)

if(NOT FE_ENABLE_ASAN AND NOT FE_ENABLE_UBSAN)
    return()
endif()

# No sanitizers in Release builds.
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message(WARNING "[FireEngine] Sanitizers disabled in Release.")
    return()
endif()

# ==========================================================================
# MSVC Configuration (Windows)
# MSVC only offers ASan; UBSan is not available on this platform.
# ==========================================================================

if(MSVC)
    if(FE_ENABLE_ASAN)
        message(STATUS "[FireEngine] ASan (MSVC) active on every target")

        # Force dynamic runtime library (DLL) required by ASan.
        set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING "" FORCE)
        
        # Enables compilation with AddressSanitizer instrumentation.
        # /Zi: Generates debug information (necessary for reports).
        add_compile_options(/fsanitize=address /Zi)

        # Disables incremental linking which is incompatible with ASan.
        add_link_options(/INCREMENTAL:NO)
    endif()

    if(FE_ENABLE_UBSAN)
        message(WARNING "[FireEngine] UBSan not supported on MSVC — ignored for targets.")
    endif()

# ==========================================================================
# GCC / Clang (WSL)
# GCC and Clang support both ASan+LSan and UBSan.
# ==========================================================================
elseif(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")

    set(SANITIZER_COMPILE_FLAGS "")
    set(SANITIZER_LINK_FLAGS "")

    # AddressSanitizer + LeakSanitizer configuration.
    if(FE_ENABLE_ASAN)
        list(APPEND SANITIZER_COMPILE_FLAGS
            -fsanitize=address            # Detects invalid memory access.
            -fsanitize=leak               # Detects memory leaks.
            -fno-omit-frame-pointer       # Preserves frame pointers for stack traces.
            -fno-optimize-sibling-calls   # Disables sibling call optimization.
        )
        list(APPEND SANITIZER_LINK_FLAGS
            -fsanitize=address            # Links ASan/LSan runtime.
            -fsanitize=leak               # Links LSan runtime.
        )
        message(STATUS "[FireEngine] ASan + LSan (GCC/Clang) active on every target")
    endif()

    # UndefinedBehaviorSanitizer configuration.
    if(FE_ENABLE_UBSAN)
        list(APPEND SANITIZER_COMPILE_FLAGS
            -fsanitize=undefined                    # Enables all default UBSan checks.
            -fsanitize=signed-integer-overflow      # Detects signed integer overflows.
            -fsanitize=null                         # Detects null pointer dereferences.
            -fsanitize=bounds                       # Detects out-of-bounds array access.
            -fno-omit-frame-pointer                 # Preserves frame pointers for stack traces.
        )
        list(APPEND SANITIZER_LINK_FLAGS
            -fsanitize=undefined                    # Links UBSan runtime.
        )
        message(STATUS "[FireEngine] UBSan (GCC/Clang) active on every target")
    endif()

    # Applies flags.
    add_compile_options(${SANITIZER_COMPILE_FLAGS})
    add_link_options(${SANITIZER_LINK_FLAGS})

# ==========================================================================
# Unknown Compiler
# ==========================================================================
else()
    message(WARNING "[FireEngine] Compiler not recognize - sanitizers ignored.")
endif()