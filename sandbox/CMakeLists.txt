# ==============================================================================
# Sandbox — Development playground
# A simple executable to test engine features during development.
# ==============================================================================

add_executable(Sandbox main.cpp)

target_link_libraries(Sandbox PRIVATE FireEngine)

target_include_directories(Sandbox PRIVATE
    ${CMAKE_SOURCE_DIR}/engine
)
