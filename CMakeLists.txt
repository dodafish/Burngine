cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

# Project name
project("Burngine 3D-Engine")

#############################################################################
# Options:

option(	BUILD_STATIC 
		"Build Burngine as static library. Set to OFF for shared build."
		OFF)
option(	BUILD_RELEASE 
		"Build Burngine with release configuration. Set to OFF for debug build."
		ON)
					
option(PRINT_SOURCE_FILES "Prints the used source files to console" ON)

#############################################################################
# Configuration:

# Determine operating system
if(${WIN32}) # Windows
    set(BURNGINE_OS_WINDOWS ON)
	message(STATUS "Compile set for Windows")
elseif(${UNIX}) # Linux
    set(BURNGINE_OS_UNIX ON)
	message(STATUS "Compile set for Linux")
else() # OS not supported
	message(FATAL_ERROR "Operating system not supported! Cannot compile Burngine on this OS.")
endif()
	
# Configure compiler flags
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    set(warnings "-Wall -Wextra -Werror")
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(warnings "/W4 /WX /EHsc")
endif()
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
    set(CMAKE_C_FLAGS   "${warnings}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()

# Set include directory for source files
# i.e. enable access with "#include <Burngine/...>"
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extlibs)

# Set output directories
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ./lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)

# Source file list. Contains all source files required
# for Burngine library compilation
set(SRC_FILES)

# Gather sources
add_subdirectory(src)

#############################################################################
# Target library

# Set build type
if(BUILD_RELEASE)
	set(CMAKE_BUILD_TYPE Release)
	set(BURNGINE_LIBRARY_POSTFIX "")
	message(STATUS "Status: Building Burngine with release configuration...")
else()
	set(CMAKE_BUILD_TYPE Debug)
	set(BURNGINE_LIBRARY_POSTFIX "-d")
	message(STATUS "Status: Building Burngine with debug configuration...")
endif()

# Check build configuration (shared or static)
if(BUILD_STATIC)
	set(BURNGINE_LIBRARY_NAME burngine-s${BURNGINE_LIBRARY_POSTFIX})
	set(BUILD_TYPE STATIC)
	message(STATUS "Status: Building Burngine as static library...")
else()
	set(BURNGINE_LIBRARY_NAME burngine${BURNGINE_LIBRARY_POSTFIX})
	set(BUILD_TYPE SHARED)
	message(STATUS "Status: Building Burngine as shared library...")
endif()

# Creation
add_library(${BURNGINE_LIBRARY_NAME} ${BUILD_TYPE} ${SRC_FILES})

# Linking
SET(EXTERN_LIBS)
if(${UNIX})
	# Unix GLEW
	
elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
	# MinGW GLEW
	LIST(APPEND EXTERN_LIBS ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/mingw/libglew32.a)
endif()

# Libs that all OS need
if(${BURNGINE_OS_WINDOWS})
	# Windows lib for WGLEW
	find_library(GDI32_LIB gdi32)
	LIST(APPEND EXTERN_LIBS ${GDI32_LIB})
	# OpenGL libs
	find_library(GLU_LIB glu32)
	find_library(OPENGL_LIB opengl32)
elseif(${BURNGINE_OS_UNIX})
	# Xwindow lib
	find_library(X11_LIB X11)
	LIST(APPEND EXTERN_LIBS ${X11_LIB})
	# OpenGL libs
	find_library(GLU_LIB glu32)
	find_library(OPENGL_LIB opengl32)
endif()

LIST(APPEND EXTERN_LIBS ${GLU_LIB})
LIST(APPEND EXTERN_LIBS ${OPENGL_LIB})

TARGET_LINK_LIBRARIES(${BURNGINE_LIBRARY_NAME} ${EXTERN_LIBS})