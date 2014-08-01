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

# Set include directory for source files
# i.e. enable access with "#include <Burngine/...>"
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)

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
	add_library(${BURNGINE_LIBRARY_NAME} STATIC ${SRC_FILES})
	message(STATUS "Status: Building Burngine as static library...")
else()
	set(BURNGINE_LIBRARY_NAME burngine${BURNGINE_LIBRARY_POSTFIX})
	add_library(${BURNGINE_LIBRARY_NAME} SHARED ${SRC_FILES})
	message(STATUS "Status: Building Burngine as shared library...")
endif()