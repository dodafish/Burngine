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

#ifndef WINDOWIMPLX11_HPP_
#define WINDOWIMPLX11_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/WindowImpl.hpp>
#include <Burngine/Window/VideoMode.hpp>

// X11 library
#include <X11/Xlib.h>
#include <X11/Xutil.h>

namespace burn {
namespace priv {

/**
 * @brief Window implementation on unix systems with X11
 */
class WindowImplX11: public WindowImpl {
public:

	/**
	 * @brief Create a window with X11
	 *
	 * @param videoMode Desired video mode
	 * @param title Desired window title
	 */
	WindowImplX11(const VideoMode& videoMode, const std::string& title, const Window::Style& style);

	/**
	 * @brief Default destructor
	 * Cleans up.
	 */
	~WindowImplX11();

	/**
	 * @brief Apply new dimensions
	 *
	 * @param dimensions Desired dimensions
	 */
	virtual void setDimensions(const Vector2i& dimensions);

	/**
	 * @brief Apply new window title
	 *
	 * @param title Desired title
	 */
	virtual void setTitle(const std::string& title);

protected:

	/**
	 * @brief Process all window events.
	 * This will add events to the queue of WindowImpl.
	 */
	virtual void processEvents();

private:

	/**
	 * @brief Disable default constructor
	 */
	WindowImplX11();

private:
	Display* m_display; ///< X11 Display (window server)
	::Window m_window; ///< X11 Window handle/object
	XClassHint m_ClassHint; ///< X11 window class
	Atom m_deleteAtom; ///< X11 delete atom. (For receiving CLOSED event)
};

} /* namespace priv */
} /* namespace burn */

#endif /* WINDOWIMPLX11_HPP_ */
