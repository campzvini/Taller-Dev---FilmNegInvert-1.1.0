cmake_minimum_required(VERSION 3.18)

project(FilmNegInvert LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

# Configuration for OpenFX SDK
# Priority: 1. OFX_ROOT environment variable or CMake variable
#            2. Local dev/openfx directory (legacy, not recommended)
#            3. Default C:/Dev/openfx
if(DEFINED OFX_ROOT)
    message(STATUS "Using OpenFX SDK from OFX_ROOT: ${OFX_ROOT}")
elseif(EXISTS "${CMAKE_SOURCE_DIR}/dev/openfx")
    set(OFX_ROOT "${CMAKE_SOURCE_DIR}/dev/openfx")
    message(WARNING "Using local OpenFX SDK at: ${OFX_ROOT}")
    message(WARNING "Consider moving SDK to external location (e.g., C:/Dev/openfx)")
    message(WARNING "and configure with: cmake .. -DOFX_ROOT=C:/Dev/openfx")
else()
    set(OFX_ROOT "C:/dev/openfx")
    message(STATUS "OFX_ROOT not defined. Defaulting to: ${OFX_ROOT}")
    message(STATUS "To specify custom location: cmake .. -DOFX_ROOT=/path/to/openfx")
endif()

set(OFX_INCLUDE_DIR "${OFX_ROOT}/include")
set(OFX_SUPPORT_DIR "${OFX_ROOT}/Support")

# Check if OpenFX headers are found
if(NOT EXISTS "${OFX_INCLUDE_DIR}/ofxCore.h")
    message(WARNING "OpenFX headers not found at ${OFX_INCLUDE_DIR}. Please ensure OpenFX SDK is downloaded to dev/openfx or set OFX_ROOT.")
endif()

# Include directories
include_directories(
    "${CMAKE_SOURCE_DIR}/include"
    "${OFX_INCLUDE_DIR}"
    "${OFX_SUPPORT_DIR}/include"
)

# Add standard library include path for MSVC
if(MSVC)
    include_directories("C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/include")
    include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/ucrt")
    include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/shared")
    include_directories("C:/Program Files (x86)/Windows Kits/10/Include/10.0.26100.0/um")
    
    # Add Windows SDK library paths
    link_directories("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/um/x64")
    link_directories("C:/Program Files (x86)/Windows Kits/10/Lib/10.0.26100.0/ucrt/x64")
    
    # Add Visual C++ library paths
    link_directories("C:/Program Files/Microsoft Visual Studio/18/Community/VC/Tools/MSVC/14.50.35717/lib/x64")
endif()

# Source files
set(SOURCES
    src/FilmNegInvertPlugin.cpp
    src/FilmNegInvertPluginFactory.cpp
    src/mainEntry.cpp
    # OpenFX Support Library sources (header-only approach)
    ${OFX_SUPPORT_DIR}/Library/ofxsCore.cpp
    ${OFX_SUPPORT_DIR}/Library/ofxsImageEffect.cpp
    ${OFX_SUPPORT_DIR}/Library/ofxsInteract.cpp
    ${OFX_SUPPORT_DIR}/Library/ofxsLog.cpp
    ${OFX_SUPPORT_DIR}/Library/ofxsMultiThread.cpp
    ${OFX_SUPPORT_DIR}/Library/ofxsParams.cpp
    ${OFX_SUPPORT_DIR}/Library/ofxsProperty.cpp
    ${OFX_SUPPORT_DIR}/Library/ofxsPropertyValidation.cpp
)

# Define the plugin library
add_library(FilmNegInvert SHARED
    ${SOURCES}
)

# Windows specific settings for OpenFX
if(WIN32)
    set_target_properties(FilmNegInvert PROPERTIES
        SUFFIX ".ofx"
        PREFIX ""
        WINDOWS_EXPORT_ALL_SYMBOLS ON
    )
    
    # Installation / Bundle Structure
    set(BUNDLE_NAME "FilmNegInvert.ofx.bundle")
    set(PLUGIN_INSTALL_DIR "C:/Program Files/Common Files/OFX/Plugins/${BUNDLE_NAME}/Contents/Win64")
    
    # Post-build command to copy the binary to the bundle structure
    # Note: This requires running VS as Admin or having write permissions to Program Files
    # If not, user should manually copy.
    add_custom_command(TARGET FilmNegInvert POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${PLUGIN_INSTALL_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:FilmNegInvert> "${PLUGIN_INSTALL_DIR}/FilmNegInvert.ofx"
        COMMENT "Installing OpenFX Plugin to ${PLUGIN_INSTALL_DIR}"
    )
endif()

# ============================================
# 6. INSTALL TARGET # Alvo de instalação
# ============================================
# Instalação automática para o diretório OFX padrão do Windows
set(OFX_INSTALL_DIR "C:/Program Files/Common Files/OFX/Plugins")
set(BUNDLE_FULL_NAME "FilmNegInvert.ofx.bundle")
set(PLUGIN_INSTALL_FULL_PATH "${OFX_INSTALL_DIR}/${BUNDLE_FULL_NAME}/Contents/Win64")

# Comando de instalação (requer privilégios de administrador)
install(TARGETS FilmNegInvert
    RUNTIME DESTINATION "${PLUGIN_INSTALL_FULL_PATH}"
    LIBRARY DESTINATION "${PLUGIN_INSTALL_FULL_PATH}"
)

# Também instala com nome correto no bundle
install(FILES "$<TARGET_FILE:FilmNegInvert>"
    DESTINATION "${PLUGIN_INSTALL_FULL_PATH}"
    RENAME "FilmNegInvert.ofx"
    OPTIONAL
)

# ============================================
# 7. CUSTOM TARGETS # Alvos personalizados
# ============================================
# Alvo para instalação rápida (sem build)
add_custom_target(install-plugin
    COMMAND ${CMAKE_COMMAND} -E make_directory "${PLUGIN_INSTALL_FULL_PATH}"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:FilmNegInvert> "${PLUGIN_INSTALL_FULL_PATH}/FilmNegInvert.ofx"
    COMMENT "Installing FilmNegInvert plugin to ${PLUGIN_INSTALL_FULL_PATH}"
    DEPENDS FilmNegInvert
)

# Alvo para criar pacote de distribuição
add_custom_target(package-plugin
    COMMAND ${CMAKE_COMMAND} -E make_directory "${CMAKE_BINARY_DIR}/package/${BUNDLE_FULL_NAME}/Contents/Win64"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different $<TARGET_FILE:FilmNegInvert> "${CMAKE_BINARY_DIR}/package/${BUNDLE_FULL_NAME}/Contents/Win64/FilmNegInvert.ofx"
    COMMAND ${CMAKE_COMMAND} -E echo "Package created at: ${CMAKE_BINARY_DIR}/package/${BUNDLE_FULL_NAME}"
    COMMENT "Creating OFX bundle package"
    DEPENDS FilmNegInvert
)
