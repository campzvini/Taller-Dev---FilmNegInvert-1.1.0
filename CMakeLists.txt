#
# ══════════════════════════════════════════════════════════════════════════════
#  FILM NEG INVERT 1.1.0 - CMAKELISTS.TXT
#  CMake build script for the Film Neg Invert OFX plugin.
#  CMAKE
# ══════════════════════════════════════════════════════════════════════════════
# Taller Lab - 2026
# https://tallerlab.com/
# VAI CORINTHIANS!!
# ══════════════════════════════════════════════════════════════════════════════
#

# ===========================================
# 1. PROJECT SETUP # Basic project configuration
# ===========================================
cmake_minimum_required(VERSION 3.10)
project(TallerNegInvert VERSION 1.1.0 LANGUAGES CXX)

# ===========================================
# 2. COMPILER SETTINGS # C++ standard and properties
# ===========================================
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    foreach(flag_var
            CMAKE_C_FLAGS CMAKE_C_FLAGS_DEBUG CMAKE_C_FLAGS_RELEASE CMAKE_C_FLAGS_MINSIZEREL CMAKE_C_FLAGS_RELWITHDEBINFO
            CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
        if(${flag_var} MATCHES "/MDd")
            string(REGEX REPLACE "/MDd" "/MTd" ${flag_var} "${${flag_var}}")
        endif()
        if(${flag_var} MATCHES "/MD")
            string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
        endif()
    endforeach()
endif()

# ===========================================
# 3. INCLUDE DIRECTORIES # Header search paths
# ===========================================
include_directories(include)
include_directories(external/openfx/include)
include_directories(external/openfx/Support/include)

# ===========================================
# 4. SOURCE FILES # Collect all .cpp files
# ===========================================
set(SOURCES
    "src/FilmNegInvertPlugin.cpp"
    "src/FilmNegInvertPluginFactory.cpp"
    "src/mainEntry.cpp"
    "external/openfx/Support/Library/ofxsCore.cpp"
    "external/openfx/Support/Library/ofxsImageEffect.cpp"
    "external/openfx/Support/Library/ofxsInteract.cpp"
    "external/openfx/Support/Library/ofxsLog.cpp"
    "external/openfx/Support/Library/ofxsMultiThread.cpp"
    "external/openfx/Support/Library/ofxsParams.cpp"
    "external/openfx/Support/Library/ofxsProperty.cpp"
    "external/openfx/Support/Library/ofxsPropertyValidation.cpp"
)

# ===========================================
# 5. LIBRARY TARGET # Define the shared library
# ===========================================
add_library(TallerNegInvert SHARED ${SOURCES})

# ===========================================
# 6. MODULE DEFINITION # Link the .def file for Windows DLL exports
# ===========================================
if(WIN32)
    set_target_properties(TallerNegInvert PROPERTIES
        LINK_FLAGS "/DEF:\"${CMAKE_SOURCE_DIR}/TallerNegInvert.def\""
    )
endif()

# ===========================================
# 7. OUTPUT # Set output directory for the library
# ===========================================
set_target_properties(TallerNegInvert PROPERTIES
    OUTPUT_NAME "FilmNegInvert"
    SUFFIX ".ofx"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
)

add_custom_command(TARGET TallerNegInvert POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/lib/Resources"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_SOURCE_DIR}/Resources/com.tallerneg.FilmNegInvert.png"
            "${CMAKE_BINARY_DIR}/lib/Resources/com.tallerneg.FilmNegInvert.png"
)

#
# ══════════════════════════════════════════════════════════════════════════════
#  END OF FILE
# ══════════════════════════════════════════════════════════════════════════════
#
