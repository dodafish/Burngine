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

#ifndef OPENGL_HPP_
#define OPENGL_HPP_

#include <Burngine/Export.hpp>

/*
 * Include GLEW properly according to target OS
 */

// Link GLEW statically
#define GLEW_STATIC

// GLEW header for all platforms with OpenGL functions
#include <GL/glew.h>

#if defined(BURNGINE_OS_WINDOWS)

// Header for windows opengl api
#include <GL/wglew.h>

#elif defined(BURNGINE_OS_LINUX)

// Header for linux opengl api
#include <GL/glxew.h>

#endif

#endif /* OPENGL_HPP_ */
