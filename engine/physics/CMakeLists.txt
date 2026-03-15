# ==============================================================================
# fe_physics — Physics System (2D + 3D)
# Rigid body dynamics, collision detection and response.
# Depends on: fe_core
# ==============================================================================

# --- 2D and 3D Physics ---
# Get all source files in the current directory and subdirectories.
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.c" "*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h" "*.hpp")

# TODO: Change INTERFACE to STATIC once implementation files is added.
add_library(fe_physics INTERFACE ${SOURCE_FILES} ${HEADER_FILES})

target_include_directories(fe_physics INTERFACE # PUBLIC
    ${CMAKE_SOURCE_DIR}/engine
)

target_link_libraries(fe_physics INTERFACE fe_core) # PUBLIC
