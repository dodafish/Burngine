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

#ifndef WINDOWHANDLE_HPP_
#define WINDOWHANDLE_HPP_

#include <Burngine/Export.hpp>

namespace burn {

// Windows' HWND is a typedef on struct HWND__*
#if defined(BURNGINE_OS_WINDOWS)
struct HWND__;
#endif

#if defined(BURNGINE_OS_WINDOWS)

// Window handle is HWND (HWND__*) on Windows
typedef HWND__* WindowHandle;

#elif defined(BURNGINE_OS_LINUX)

// Window handle is Window (unsigned long) on Unix - X11
typedef unsigned long WindowHandle;

#endif

}

#endif /* WINDOWHANDLE_HPP_ */
