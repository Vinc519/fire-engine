# ==============================================================================
# fe_core — Core Systems
# Corresponds to the "Core Systems" layer in the engine architecture.
# No dependencies on other engine modules.
# ==============================================================================

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.c" "*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h" "*.hpp")

add_library(fe_core STATIC ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(fe_core PUBLIC
    ${CMAKE_SOURCE_DIR}/engine
)

# Platform-specific compile definitions
if(FE_PLATFORM_WINDOWS)
    target_compile_definitions(fe_core INTERFACE FE_PLATFORM_WINDOWS)
elseif(FE_PLATFORM_LINUX)
    target_compile_definitions(fe_core INTERFACE FE_PLATFORM_LINUX)
endif()

# Compiler warnings
if(MSVC)
    target_compile_options(fe_core PRIVATE /W4)
else()
    target_compile_options(fe_core PRIVATE -Wall -Wextra -Wpedantic)
endif()
