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

#ifndef WGLCONTEXT_HPP_
#define WGLCONTEXT_HPP_

#include <Burngine/Window/GlContext.hpp>
#include <Burngine/OpenGL.hpp>
#include <windows.h>

namespace burn {
namespace priv {

class WglContext : public GlContext {
public:

	/**
	 * @brief Create a context based on an internal fake window
	 */
	WglContext();

	/**
	 * @brief Create a context based on a given window
	 */
	WglContext(const Window* window);

	/**
	 * @brief Cleans up GLEW and window classes if necessary
	 *
	 * @note After deletion there is NO CONTEXT ACTIVE! You have
	 * to activate another one again!
	 */
	~WglContext();

private:

	/**
	 * @brief GLEW will be initialized and ready for use
	 *
	 * @return True on success
	 */
	bool initGlew();

	/**
	 * @brief Create a fake window and use that
	 *
	 * @param hWnd Handle that stores the fake window
	 *
	 * @return True if window was created
	 *
	 * @note You have to destroy the window afterwards
	 */
	bool createFakeWindow(HWND& hWnd);

	/**
	 * @brief Create the OpenGL context attached to the window.
	 * Make sure to create a fake window if no other window was
	 * provided.
	 */
	void createContext();

private:
	HWND m_windowHandle;    ///< Attached window
	HGLRC m_hRC; ///< The context actually
	HDC m_hDC; ///< Device context
	bool m_isWindowOwner; ///< True if WglContext owns the window
};

} /* namespace priv */
} /* namespace burn */
#endif /* WGLCONTEXT_HPP_ */
