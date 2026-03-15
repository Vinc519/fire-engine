# ==============================================================================
# Fire Engine — Engine Module
# Builds each subsystem as a static library and aggregates them into
# a single "FireEngine" interface target for easy linking.
# ==============================================================================

# --- Core Systems (bottom of the stack, no engine dependencies) ---
add_subdirectory(core)

# --- Platform Independence Layer ---
add_subdirectory(platform)

# --- High-level Systems (depend on core + platform) ---
add_subdirectory(renderer)
add_subdirectory(input)
add_subdirectory(audio)
add_subdirectory(physics)

# ==============================================================================
# Umbrella target: link against "FireEngine" to get everything
# ==============================================================================
add_library(FireEngine INTERFACE)

target_link_libraries(FireEngine INTERFACE
    fe_core
    fe_platform
    fe_renderer
    fe_input
    fe_audio
    fe_physics
)
