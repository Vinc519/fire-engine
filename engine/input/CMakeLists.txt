# ==============================================================================
# fe_input — Input System
# Customizable input mapping for keyboard, mouse, and gamepad.
# Depends on: fe_core, fe_platform
# ==============================================================================

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.c" "*.cpp")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.h" "*.hpp")

# TODO: Change INTERFACE to STATIC once implementation files is added.
add_library(fe_input INTERFACE ${SOURCE_FILES} ${HEADER_FILES})

if(FE_PLATFORM_WINDOWS)
    target_sources(fe_input PRIVATE
        # TODO: platform/Input_Windows.cpp
    )
elseif(FE_PLATFORM_LINUX)
    target_sources(fe_input PRIVATE
        # TODO: platform/Input_Linux.cpp
    )
endif()

target_include_directories(fe_input INTERFACE # PUBLIC 
    ${CMAKE_SOURCE_DIR}/engine
)

target_link_libraries(fe_input INTERFACE fe_core fe_platform) # PUBLIC
