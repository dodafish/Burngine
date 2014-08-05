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

#include <Burngine/Window/Win32/WglContext.hpp>
#include <iostream>

namespace {

bool isFakeClassRegistered = false;
const char fakeClassName[] = "FAKE_CLASS_NAME";
unsigned int count = 0;

/////////////////////////////////////////////////////////////////////////////

LRESULT CALLBACK fakeWndProc(	HWND hWnd,
								UINT uiMsg,
								WPARAM wParam,
								LPARAM lParam) {
	// Default window procedure
	// We don't handle anything ourselves
	return DefWindowProc(hWnd, uiMsg, wParam, lParam);
}

}

namespace burn {
namespace priv {

WglContext::WglContext() :
m_windowHandle(NULL),
m_hRC(NULL),
m_hDC(NULL),
m_isWindowOwner(true) {

	// First context?
	if(count == 0){
		initGlew();
	}

	// Create a fake window to base the context on
	if(!createFakeWindow(m_windowHandle))
		return;

	createContext();

}

WglContext::WglContext(const Window* window) :
m_windowHandle(NULL),
m_hRC(NULL),
m_hDC(NULL),
m_isWindowOwner(true) {

	// First context?
	if(count == 0){
		initGlew();
	}

	// Get handle
	m_windowHandle = (HWND)window->getWindowHandle();

	createContext();

}

WglContext::~WglContext() {

	// Last context?
	if(count == 1){
		if(isFakeClassRegistered)
			// Unregister windowclass
			UnregisterClass(fakeClassName, GetModuleHandle(NULL));
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(m_hRC);

	if(m_isWindowOwner){
		DestroyWindow(m_windowHandle);
	}

}

void WglContext::createContext() {

	// Get device context
	m_hDC = GetDC(m_windowHandle);

	// Create a PFD. Even if its unnecessary for OpenGl 3.0+
	PIXELFORMATDESCRIPTOR pfd;

	if(WGLEW_ARB_create_context && WGLEW_ARB_pixel_format){

		// Try creating a context.
		// Create the highest OpenGL version context
		// Start at OpenGL 4.4
		int major = 4;
		int minor = 4;

		bool success = false;
		while(!success){

			std::cout << "Trying to create context with OpenGL version " << major << "." << minor << "\n";

			success = true;

			const int iPixelFormatAttribList[] = {
			WGL_DRAW_TO_WINDOW_ARB,
			GL_TRUE,
			WGL_SUPPORT_OPENGL_ARB,
			GL_TRUE,
			WGL_DOUBLE_BUFFER_ARB,
			GL_TRUE,
			WGL_PIXEL_TYPE_ARB,
			WGL_TYPE_RGBA_ARB,
			WGL_COLOR_BITS_ARB,
			32,
			WGL_DEPTH_BITS_ARB,
			24,
			WGL_STENCIL_BITS_ARB,
			8,
			0    // End of attributes list
			};
			int iContextAttribs[] = {
			WGL_CONTEXT_MAJOR_VERSION_ARB,
			major,
			WGL_CONTEXT_MINOR_VERSION_ARB,
			minor,
			WGL_CONTEXT_FLAGS_ARB,
			WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
			0    // End of attributes list
			};

			int iPixelFormat, iNumFormats;
			wglChoosePixelFormatARB(m_hDC, iPixelFormatAttribList, NULL, 1, &iPixelFormat, (UINT*)&iNumFormats);

			// PFD seems to be only redundant parameter now
			if(!SetPixelFormat(m_hDC, iPixelFormat, &pfd)){
				success = false;
			}else{
				m_hRC = wglCreateContextAttribsARB(m_hDC, 0, iContextAttribs);
				if(!m_hRC)
					success = false;
			}

			if(!success){

				std::cout << "Unable to create context with OpenGL version " << major << "." << minor << "\n";

				// We could not create that version. Try a lower one
				if(minor == 0){
					// In this case set to 3.9
					major--;
					minor = 9;
				}else{
					minor--;
				}

				//Are we still 3.3+ ?
				if(major <= 3 && minor < 3){
					//We are lower 3.3. Abort!
					std::cerr << "Failed to create OpenGL 3.3+ context! Try updating your graphic driver.\n";
					return;
				}

			}

		}

		// Context creation succeeded!
		std::cout << "Created context. OpenGL version: " << major << "." << minor << "\n";

		// Make it current (use it)
		wglMakeCurrent(m_hDC, m_hRC);

	}else{
		std::cerr << "Failed to create OpenGL 3.3+ context! Try updating your graphic driver.\n";
	}

}

bool WglContext::createFakeWindow(HWND& hWnd) {

	if(!isFakeClassRegistered){
		// Register a class for the fake window
		WNDCLASSEX wc;
		wc.cbSize = sizeof(WNDCLASSEX);
		wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
		wc.lpfnWndProc = fakeWndProc;
		wc.cbClsExtra = 0;
		wc.cbWndExtra = 0;
		wc.hInstance = GetModuleHandle(NULL);
		wc.hIcon = LoadIcon(wc.hInstance, (IDI_APPLICATION ));
		wc.hIconSm = LoadIcon(wc.hInstance, (IDI_APPLICATION ));
		wc.hCursor = LoadCursor(NULL, IDC_ARROW);
		wc.hbrBackground = (HBRUSH)(COLOR_MENU + 1);
		wc.lpszMenuName = NULL;
		wc.lpszClassName = fakeClassName;
		if(!RegisterClassEx(&wc)){
			std::cerr << "Failed to register fake window class!\n";
			return false;
		}
		isFakeClassRegistered = true;
	}

	hWnd = CreateWindow(fakeClassName, "FAKE", WS_DISABLED | WS_MINIMIZE,
	0, 0, CW_USEDEFAULT, CW_USEDEFAULT, NULL,
	NULL, GetModuleHandle(NULL), NULL);

	if(!hWnd){
		std::cerr << "Failed to create fake window! Cannot init GLEW.\n";
		return false;
	}

// Hide the fake window
	ShowWindow(hWnd, SW_HIDE);

	return true;
}

bool WglContext::initGlew() {

// Are there already contexts?
	if(count > 0)
		return true;

	/*
	 * To init GLEW we need a fake window with an old opengl context.
	 * This way GLEW can fetch his function pointers
	 */

// Create fake window
	HWND hWndFake = NULL;
	if(!createFakeWindow(hWndFake)){
		return false;
	}

	HDC hDC = GetDC(hWndFake);

// First, choose false pixel format

	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
	pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cDepthBits = 32;
	pfd.iLayerType = PFD_MAIN_PLANE;

	int iPixelFormat = ChoosePixelFormat(hDC, &pfd);
	if(iPixelFormat == 0)
		return false;

	if(!SetPixelFormat(hDC, iPixelFormat, &pfd))
		return false;

// Create the false, old style context (OpenGL 2.1 and before)

	HGLRC hRCFake = wglCreateContext(hDC);
	wglMakeCurrent(hDC, hRCFake);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK){
		std::cerr << "Couldn't initialize GLEW!\n";
		return false;
	}else{
		std::cout << "GLEW initialized.\n";
	}

	wglMakeCurrent(NULL, NULL);
	wglDeleteContext(hRCFake);
	DestroyWindow(hWndFake);

	return true;
}

} /* namespace priv */
} /* namespace burn */
