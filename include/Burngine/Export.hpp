//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU LGPL v3 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This library is free software; you can redistribute it and/or
//    modify it under the terms of the GNU Lesser General Public
//    License as published by the Free Software Foundation;
//    version 3.0 of the License
//
//    This library is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//    Lesser General Public License for more details.
//
//    You should have received a copy of the GNU Lesser General Public
//    License along with this library; if not, write to the Free Software
//    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301
//    USA
//
//////////////////////////////////////////////////////////////////////////////

#ifndef EXPORT_HPP_
#define EXPORT_HPP_

// Linker settings for external libraries:
#define PTW32_STATIC_LIB // Link pthread statically////////////////////////////////////////////////////////////
// Identify the operating system
// see http://nadeausoftware.com/articles/2012/01/c_c_tip_how_use_compiler_predefined_macros_detect_operating_system
////////////////////////////////////////////////////////////
#if defined(_WIN32)

// Windows
#define BURNGINE_OS_WINDOWS
#ifndef NOMINMAX
#define NOMINMAX
#endif

#elif defined(__APPLE__) && defined(__MACH__)

// Apple platform, see which one it is
#include <TargetConditionals.h>

#if TARGET_OS_MAC
// MacOS
#define BURNGINE_OS_MACOS

#else
// Unsupported Apple system
#error This Apple operating system is not supported by Burngine

#endif

#elif defined(__unix__)
#if defined(__linux__)
// Linux
#define BURNGINE_OS_LINUX

#else
// Unsupported UNIX system
#error This UNIX operating system is not supported by Burngine
#endif

#else

// Unsupported system
#error This operating system is not supported by Burngine

#endif

/*
 * ==========================================
 */

#if !defined(BURNGINE_STATIC)

#if defined(BURNGINE_OS_WINDOWS)

// Windows compilers need specific (and different) keywords for export and import
#define BURNGINE_API_EXPORT __declspec(dllexport)
#define BURNGINE_API_IMPORT __declspec(dllimport)

// For Visual C++ compilers, we also need to turn off this annoying C4251 warning
#if defined(_MSC_VER)

#pragma warning(disable : 4251)

#endif

#else // Linux and Mac OS X#define BURNGINE_API_EXPORT
#define BURNGINE_API_IMPORT

#if (__GNUC__ >= 4)

// GCC 4 has special keywords for showing/hidding symbols,
// the same keyword is used for both importing and exporting
#define BURNGINE_API_EXPORT __attribute__ ((__visibility__ ("default")))
#define BURNGINE_API_IMPORT __attribute__ ((__visibility__ ("default")))

#endif

#endif

#else

// Static build doesn't need import/export macros
#define BURNGINE_API_EXPORT
#define BURNGINE_API_IMPORT

#endif

namespace burn {
// All "common" platforms use the same size for char, short and int
// (basically there are 3 types for 3 sizes, so no other match is possible),
// we can use them without doing any kind of check

// 8 bits integer types
	typedef signed char Int8;
	typedef unsigned char Uint8;

// 16 bits integer types
	typedef signed short Int16;
	typedef unsigned short Uint16;

// 32 bits integer types
	typedef signed int Int32;
	typedef unsigned int Uint32;

// 64 bits integer types
#if defined(_MSC_VER)
	typedef signed __int64 Int64;
	typedef unsigned __int64 Uint64;
#else
	typedef signed long long Int64;
	typedef unsigned long long Uint64;
#endif

// nullpointer macro NULL:
// Comes to effect when nothing like <cstdlib> was imported
#if !defined(NULL)
#define NULL 0
#endif

}

#endif /* EXPORT_HPP_ */
