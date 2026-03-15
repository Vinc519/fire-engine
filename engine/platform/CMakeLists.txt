# ==============================================================================
# fe_platform — Platform Independence Layer
# Abstracts OS-specific behaviour behind a common API.
# Depends on: fe_core
# ==============================================================================

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.cpp" "*.c")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.hpp" "*.h")

# Remove platform-specific files
list(FILTER SOURCE_FILES EXCLUDE REGEX "^.*_(Windows|Linux)\.(cpp|c)$")

# Adding platform-specific sources
if(FE_PLATFORM_WINDOWS)
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS "*_Windows.cpp" "*_Windows.c")
elseif(FE_PLATFORM_LINUX)
    file(GLOB_RECURSE PLATFORM_SOURCES CONFIGURE_DEPENDS "*_Linux.cpp" "*_Linux.c")
endif()

list(APPEND SOURCE_FILES ${PLATFORM_SOURCES})

# TODO: Change INTERFACE to STATIC once implementation files is added.
add_library(fe_platform INTERFACE ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(fe_platform INTERFACE # PUBLIC
    ${CMAKE_SOURCE_DIR}/engine
)

target_link_libraries(fe_platform INTERFACE fe_core) # PUBLIC

# Link OS threading libraries
find_package(Threads REQUIRED)
target_link_libraries(fe_platform INTERFACE Threads::Threads) # PRIVATE
