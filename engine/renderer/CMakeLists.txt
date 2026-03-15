# ==============================================================================
# fe_renderer — Rendering System
# Provides a backend-agnostic rendering API with concrete implementations
# for Vulkan, DirectX 12, and OpenGL.
# Depends on: fe_core, fe_platform
# ==============================================================================

# TODO: Change INTERFACE to STATIC once implementation files is added.
add_library(fe_renderer INTERFACE)

# --- Common / Abstract API (always compiled) ---
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS "*.cpp" "*.c")
file(GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS "*.hpp" "*.h")

list(FILTER SOURCE_FILES EXCLUDE REGEX ".*(Vulkan|DX12|OGL).*\.(cpp|c)$")
list(FILTER HEADER_FILES EXCLUDE REGEX ".*(Vulkan|DX12|OGL).*\.(hpp|h)$")

# --- Vulkan Backend ---
if(FE_ENABLE_VULKAN)
    if(WIN32)
        # On Windows, VULKAN_SDK environnement variable needs to be defined by the LunarG installer.
        if(NOT DEFINED ENV{VULKAN_SDK})
            message(FATAL_ERROR "[FireEngine] VULKAN_SDK not found on Windows.\n")
        endif()
        # Explicit hint to guide find_package.
        set(Vulkan_ROOT "$ENV{VULKAN_SDK}" CACHE PATH "Root of the Vulkan SDK" FORCE)

    else()
        if(DEFINED ENV{VULKAN_SDK})
            set(Vulkan_ROOT "$ENV{VULKAN_SDK}" CACHE PATH "Root of the Vulkan SDK" FORCE)
        else()
            message(WARNING "[FireEngine] VULKAN_SDK not found in environment variables. Trying other locations.")
        endif()
    endif()

    find_package(Vulkan REQUIRED)

    message(STATUS "[FireEngine] Vulkan found : ${Vulkan_VERSION}")
    message(STATUS "             Headers : ${Vulkan_INCLUDE_DIRS}")
    message(STATUS "             Lib     : ${Vulkan_LIBRARIES}")

    file(GLOB_RECURSE VULKAN_SOURCE_FILES CONFIGURE_DEPENDS "Vulkan*.cpp" "Vulkan*.c")
    file(GLOB_RECURSE VULKAN_HEADER_FILES CONFIGURE_DEPENDS "Vulkan*.hpp" "Vulkan*.h")
    list(APPEND SOURCE_FILES ${VULKAN_SOURCE_FILES})
    list(APPEND HEADER_FILES ${VULKAN_HEADER_FILES})

    target_link_libraries(fe_renderer INTERFACE Vulkan::Vulkan) # PRIVATE
    target_compile_definitions(fe_renderer INTERFACE FE_RENDERER_VULKAN) # PUBLIC
endif()

# --- DirectX 12 Backend ---
if(FE_ENABLE_DX12)
    file(GLOB_RECURSE DX12_SOURCE_FILES CONFIGURE_DEPENDS "DX12*.cpp" "DX12*.c")
    file(GLOB_RECURSE DX12_HEADER_FILES CONFIGURE_DEPENDS "DX12*.hpp" "DX12*.h")
    list(APPEND SOURCE_FILES ${DX12_SOURCE_FILES})
    list(APPEND HEADER_FILES ${DX12_HEADER_FILES})

    target_link_libraries(fe_renderer PRIVATE d3d12 dxgi d3dcompiler)
    target_compile_definitions(fe_renderer PUBLIC FE_RENDERER_DX12)
endif()

# --- OpenGL Backend ---
if(FE_ENABLE_OPENGL)
    find_package(OpenGL REQUIRED)

    file(GLOB_RECURSE OGL_SOURCE_FILES CONFIGURE_DEPENDS "OGL*.cpp" "OGL*.c")
    file(GLOB_RECURSE OGL_HEADER_FILES CONFIGURE_DEPENDS "OGL*.hpp" "OGL*.h")
    list(APPEND SOURCE_FILES ${OGL_SOURCE_FILES})
    list(APPEND HEADER_FILES ${OGL_HEADER_FILES})

    target_link_libraries(fe_renderer PRIVATE OpenGL::GL)
    target_compile_definitions(fe_renderer PUBLIC FE_RENDERER_OPENGL)
endif()

# Link files and dependencies
target_sources(fe_renderer INTERFACE ${SOURCE_FILES} ${HEADER_FILES}) # PRIVATE

target_include_directories(fe_renderer INTERFACE # PUBLIC
    ${CMAKE_SOURCE_DIR}/engine
)

target_link_libraries(fe_renderer INTERFACE fe_core fe_platform) # PUBLIC
