# =============================================================================
# cmake/StaticAnalysis.cmake — Fire Engine
#
# Registers three CMake targets independent of the normal build:
#
#   cmake --build <preset> --target tidy           ← clang-tidy on entire project
#   cmake --build <preset> --target format         ← applies clang-format
#   cmake --build <preset> --target format-check   ← checks formatting (CI)
#
# The FE_ENABLE_CLANG_TIDY option additionally integrates clang-tidy into each
# compilation (warnings appear in the IDE error list). Enabled only in Debug
# presets — slows down compilation.
# =============================================================================

option(FE_ENABLE_CLANG_TIDY "Integrate clang-tidy into every compilation" OFF)

# ==============================================================================
# Locate Executables
# ==============================================================================
find_program(CLANG_TIDY_EXECUTABLE
    NAMES clang-tidy clang-tidy-18
    DOC   "Path to the clang-tidy executable"
)

find_program(CLANG_FORMAT_EXECUTABLE
    NAMES clang-format clang-format-18
    DOC   "Path to the clang-format executable"
)

# ==============================================================================
# Collect All Project Source Files
# ==============================================================================
file(GLOB_RECURSE FE_ALL_SOURCES
    CONFIGURE_DEPENDS
    "${CMAKE_SOURCE_DIR}/engine/*.h"
    "${CMAKE_SOURCE_DIR}/engine/*.cpp"
    "${CMAKE_SOURCE_DIR}/sandbox/*.h"
    "${CMAKE_SOURCE_DIR}/sandbox/*.cpp"
    "${CMAKE_SOURCE_DIR}/tests/*.h"
    "${CMAKE_SOURCE_DIR}/tests/*.cpp"
)

# ==============================================================================
# clang-tidy — Build Integration (Optional)
# ==============================================================================
if(FE_ENABLE_CLANG_TIDY)
    if(CLANG_TIDY_EXECUTABLE)
        message(STATUS "[FireEngine] clang-tidy integrated into build: ${CLANG_TIDY_EXECUTABLE}")
        set(CMAKE_CXX_CLANG_TIDY
            "${CLANG_TIDY_EXECUTABLE}"
            "--config-file=${CMAKE_SOURCE_DIR}/.clang-tidy"
            # "--warnings-as-errors=*"
            PARENT_SCOPE
        )
    else()
        message(WARNING
            "[FireEngine] FE_ENABLE_CLANG_TIDY=ON but clang-tidy not found.\n"
            "  Windows : winget install LLVM.LLVM\n"
            "  Linux   : sudo apt install clang-tidy\n"
            "  macOS   : brew install llvm"
        )
    endif()
endif()

# ==============================================================================
# Target "tidy" — Manual clang-tidy Analysis
# ==============================================================================
if(CLANG_TIDY_EXECUTABLE)
    add_custom_target(tidy
        COMMAND ${CLANG_TIDY_EXECUTABLE}
            --config-file=${CMAKE_SOURCE_DIR}/.clang-tidy
            -p ${CMAKE_BINARY_DIR}
            ${FE_ALL_SOURCES}
        COMMENT "[FireEngine] clang-tidy — analysis in progress..."
        VERBATIM
    )
    message(STATUS "[FireEngine] Target 'tidy'          : cmake --build <preset> --target tidy")
else()
    message(WARNING
        "[FireEngine] clang-tidy not found — 'tidy' target not available.\n"
        "  Windows : winget install LLVM.LLVM\n"
        "  Linux   : sudo apt install clang-tidy\n"
        "  macOS   : brew install llvm"
    )
endif()

# ==============================================================================
# Target "format" — Apply clang-format to All Files
#
# Reformats all source files in-place according to the .clang-format 
# configuration file at the project root.
# ==============================================================================
if(CLANG_FORMAT_EXECUTABLE)
    add_custom_target(format
        COMMAND ${CLANG_FORMAT_EXECUTABLE}
            --style=file
            -i              # Modifies files in place
            ${FE_ALL_SOURCES}
        COMMENT "[FireEngine] clang-format — formatting in progress..."
        VERBATIM
    )

    # ==============================================================================
    # Target "format-check" — Verify formatting without modification
    #
    # Checks if files comply with .clang-format without modifying them.
    # ==============================================================================
    add_custom_target(format-check
        COMMAND ${CLANG_FORMAT_EXECUTABLE}
            --style=file
            --dry-run       # Performs checks without modifying files.
            --Werror        # Treats formatting violations as errors.
            ${FE_ALL_SOURCES}
        COMMENT "[FireEngine] clang-format — verification in progress..."
        VERBATIM
    )

    message(STATUS "[FireEngine] Target 'format'        : cmake --build <preset> --target format")
    message(STATUS "[FireEngine] Target 'format-check'  : cmake --build <preset> --target format-check")
else()
    message(WARNING
        "[FireEngine] clang-format not found — 'format' and 'format-check' targets not available.\n"
        "  Windows : winget install LLVM.LLVM\n"
        "  Linux   : sudo apt install clang-format\n"
        "  macOS   : brew install llvm"
    )
endif()