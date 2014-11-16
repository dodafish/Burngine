cmake_minimum_required(VERSION 2.8 FATAL_ERROR)

#############################################################################
# Project name
#############################################################################
project("Burngine")
#############################################################################

#############################################################################
# Options:
#############################################################################
option(	BURNGINE_STATIC 
		"Build Burngine as static library. Set to OFF for shared build."
		OFF)
option(	BURNGINE_RELEASE 
		"Build Burngine with release configuration. Set to OFF for debug build."
		ON)
					
option(	IGNORE_WARNINGS 
		"Ignore all compiler warnings. (Only use if warnings are unnecessarily blocking compilation"
		OFF)
					
option(PRINT_SOURCE_FILES "Prints the used source files to console" ON)
#############################################################################

#############################################################################
# Determine operating system
#############################################################################
if(${WIN32}) # Windows
    set(BURNGINE_OS_WINDOWS ON)
	message(STATUS "Compile set for Windows")
elseif(${UNIX}) # Linux
	if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
		set(BURNGINE_OS_LINUX ON)
		message(STATUS "Compile set for Linux")
	elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
		set(BURNGINE_OS_MACOS ON)
		message(STATUS "Compile set for MacOS")
	else()
		message(FATAL_ERROR "This Unix system is not supported!")
	endif()
else() # OS not supported
	message(FATAL_ERROR "Operating system not supported! Cannot compile Burngine on this OS.")
endif()
#############################################################################

#############################################################################
# Configure compiler flags and determine compiler
#############################################################################
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU" OR
    "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
	
	set(cpp11flag "-std=gnu++11")
	
	if(NOT ${IGNORE_WARNINGS})
		set(warnings "-Wall -Wextra -Werror")
	else()
		set(warnings "-W")
	endif()
	
	if(${BURNGINE_OS_WINDOWS})
		set(BURNGINE_COMPILER_MINGW ON)
	elseif(${BURNGINE_OS_LINUX})
		set(BURNGINE_COMPILER_GNU_GCC ON)
	else()
		message(FATAL_ERROR "Cannot determine GNU/Clang compiler!")
	endif()
	
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
	if(NOT ${IGNORE_WARNINGS})
		set(warnings "/W4 /WX /EHsc")
	else()
		set(warnings "/W0")
	endif()
	
	set(BURNGINE_COMPILER_MSVC ON)
	
endif()

#############################################################################
# Set compiler warning flags
#############################################################################
if (NOT CONFIGURED_ONCE)
    set(CMAKE_CXX_FLAGS "${warnings} ${cpp11flag}"
        CACHE STRING "Flags used by the compiler during all build types." FORCE)
endif()
#############################################################################

#############################################################################
# Set source files' include directories
#############################################################################
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/extlibs)
#############################################################################

#############################################################################
# Set output directories
#############################################################################
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ./lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ./lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ./bin)
#############################################################################

#############################################################################
# Get all source files
#############################################################################
set(SRC_FILES)
add_subdirectory(src)
#############################################################################

#############################################################################
# Set build type
#############################################################################
if(BURNGINE_RELEASE)
	# Release
	set(CMAKE_BUILD_TYPE Release)
	set(BURNGINE_LIBRARY_POSTFIX "")
	message(STATUS "Status: Building Burngine with release configuration...")
else()
	# Debug
	set(CMAKE_BUILD_TYPE Debug)
	set(BURNGINE_LIBRARY_POSTFIX "-d")
	message(STATUS "Status: Building Burngine with debug configuration...")
endif()
#############################################################################

#############################################################################
# Set build configuration
#############################################################################
if(BURNGINE_STATIC)
	# Static
	set(BURNGINE_LIBRARY_NAME burngine-s${BURNGINE_LIBRARY_POSTFIX})
	set(BUILD_TYPE STATIC)
	message(STATUS "Status: Building Burngine as static library...")
else()
	# Shared
	set(BURNGINE_LIBRARY_NAME burngine${BURNGINE_LIBRARY_POSTFIX})
	set(BUILD_TYPE SHARED)
	message(STATUS "Status: Building Burngine as shared library...")
endif()
#############################################################################

#############################################################################
# Build Burngine!
#############################################################################
add_library(${BURNGINE_LIBRARY_NAME} ${BUILD_TYPE} ${SRC_FILES})
#############################################################################

#############################################################################
# Link dependencies
#############################################################################

if(${BURNGINE_OS_WINDOWS})

	if(${BURNGINE_COMPILER_MINGW})
	
		SET(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/mingw)
		
		# Libs used by Burngine
		find_library(GLU_LIB glu32)
		find_library(OPENGL_LIB opengl32)
		find_library(PTHREAD_LIB NAMES pthread PATHS ${LIB_PATH} NO_DEFAULT_PATH)
		find_library(GLEW_LIB NAMES glew32 PATHS ${LIB_PATH} NO_DEFAULT_PATH)
		find_library(SOIL_LIB NAMES SOIL PATHS ${LIB_PATH} NO_DEFAULT_PATH)
		find_library(ASSIMP_LIB NAMES assimp PATHS ${LIB_PATH} NO_DEFAULT_PATH)
		
		# Additional libs
		find_library(GDI32_LIB gdi32)
		LIST(APPEND EXTERN_LIBS ${GDI32_LIB})
		
	else() # MSVC
	
		SET(LIB_PATH ${CMAKE_CURRENT_SOURCE_DIR}/extlibs/msvc)
	
		# Libs used by Burngine
		find_library(GLU_LIB GlU32)
		find_library(OPENGL_LIB opengl32)
		find_library(GLEW_LIB NAMES glew32s PATHS ${LIB_PATH} NO_DEFAULT_PATH)
		find_library(PTHREAD_LIB NAMES pthread PATHS ${LIB_PATH} NO_DEFAULT_PATH)
		find_library(SOIL_LIB NAMES SOIL PATHS ${LIB_PATH} NO_DEFAULT_PATH)
		find_library(ASSIMP_LIB NAMES assimp PATHS ${LIB_PATH} NO_DEFAULT_PATH)
		
		# Additional libs
		LIST(APPEND EXTERN_LIBS gdi32.lib)
		
	endif()
	
elseif(${BURNGINE_OS_LINUX})

	# Xwindow lib
	find_library(X11_LIB X11)
	LIST(APPEND EXTERN_LIBS ${X11_LIB})
	# OpenGL libs
	find_library(GLU_LIB glu32)
	find_library(OPENGL_LIB opengl32)
	
endif()

LIST(APPEND EXTERN_LIBS ${GLEW_LIB})
LIST(APPEND EXTERN_LIBS ${GLU_LIB})
LIST(APPEND EXTERN_LIBS ${OPENGL_LIB})
LIST(APPEND EXTERN_LIBS ${PTHREAD_LIB})
LIST(APPEND EXTERN_LIBS ${SOIL_LIB})

# 3rd party libraries
LIST(APPEND EXTERN_LIBS ${ASSIMP_LIB})

# Link libraries!
TARGET_LINK_LIBRARIES(${BURNGINE_LIBRARY_NAME} ${EXTERN_LIBS})