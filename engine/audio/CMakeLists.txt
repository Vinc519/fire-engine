# ==============================================================================
# fe_audio — Audio System
# Sound loading, playback, and 3D positional audio.
# Depends on: fe_core, fe_platform
# ==============================================================================

# Get all source files in the current directory and subdirectories.
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.c" "*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h" "*.hpp")

# TODO: Change INTERFACE to STATIC once implementation files is added.
add_library(fe_audio INTERFACE ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(fe_audio INTERFACE
    ${CMAKE_SOURCE_DIR}/engine
)

target_link_libraries(fe_audio INTERFACE fe_core fe_platform)

# TODO: Add platform-specific audio libraries (e.g., XAudio2 for Windows, ALSA/PulseAudio/PipeWire for Linux)
