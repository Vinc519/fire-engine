# ==============================================================================
# Sandbox — Development playground
# A simple executable to test engine features during development.
# ==============================================================================

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.c" "*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h" "*.hpp")

add_executable(Sandbox ${SOURCE_FILES} ${HEADER_FILES})

target_link_libraries(Sandbox PRIVATE FireEngine)

target_include_directories(Sandbox PRIVATE
    ${CMAKE_SOURCE_DIR}/engine
)
