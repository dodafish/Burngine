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

#include <Burngine/Window/Win32/WindowImplWin32.hpp>
#include <Burngine/System/Error.hpp>

#include <iostream>
#include <string>
#include <tchar.h>
#include <map>

namespace {
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

	unsigned int windowCount = 0;
	LPCTSTR className = TEXT("Burngine_WinClass");

	std::map<HWND, burn::priv::WindowImplWin32*> windowMap;

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
} /* namespace */

namespace burn {
	namespace priv {

		WindowImplWin32::WindowImplWin32(	const VideoMode& videoMode,
											const std::string& title,
											const Window::Style& style,
											bool fullscreen) :
		m_windowHandle(NULL) {

			windowCount++;

			// Register window class if this is the first window
			if(windowCount == 1)
				registerWindowClass();

			// Set a titlebar as minimum for style
			DWORD windowStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
			if(style != Window::FIXED_SIZE){
				windowStyle |= WS_MAXIMIZEBOX;    // Maximize button
				windowStyle |= WS_SIZEBOX;    // Sizeable border
			}

			int width = static_cast<int>(videoMode.getWidth());
			int height = static_cast<int>(videoMode.getHeight());

			RECT rectangle = {
			0,
			0,
			width,
			height };
			AdjustWindowRect(&rectangle, windowStyle, false);
			width = rectangle.right - rectangle.left;
			height = rectangle.bottom - rectangle.top;

			m_windowHandle = CreateWindow(className, title.c_str(),
			windowStyle,
			CW_USEDEFAULT, CW_USEDEFAULT, width, height,
			NULL,
			NULL,
			GetModuleHandle(NULL),
			NULL);

			if(!m_windowHandle)
				burnErr("Failed creating Win32 window!");

			// Open the window
			ShowWindow(m_windowHandle, SW_SHOW);

			// Wake the window
			if(!UpdateWindow(m_windowHandle))
				burnErr("Call to UpdateWindow failed!");

			// Map the window for event process call
			windowMap.insert(std::pair<HWND, WindowImplWin32*>(m_windowHandle, this));

			if(fullscreen)
				switchToFullscreen(videoMode);

		}

		WindowImplWin32::~WindowImplWin32() {

			// Destroy the win
			if(m_windowHandle){
				if(!DestroyWindow(m_windowHandle)){
					burnErr("Failed to destroy Win32 window!");
				}
			}

			// Last window?
			if(windowCount == 1){
				if(!UnregisterClass(className, GetModuleHandle(NULL)))
					burnErr("Failed to unregister Win32 class!");
			}

			windowCount--;
		}

		void WindowImplWin32::switchToFullscreen(const VideoMode& videoMode) {

			DEVMODE devMode;
			devMode.dmSize = sizeof(devMode);
			devMode.dmPelsWidth = videoMode.getWidth();
			devMode.dmPelsHeight = videoMode.getHeight();
			devMode.dmBitsPerPel = 32;
			devMode.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_BITSPERPEL;

			// Apply fullscreen mode
			if(ChangeDisplaySettings(&devMode,
			CDS_FULLSCREEN) != DISP_CHANGE_SUCCESSFUL){
				burnErr("Failed to change display mode to fullscreen.");
			}

			// Make window flahs compatible with fullscreen mode
			SetWindowLong(m_windowHandle,
			GWL_STYLE,
							WS_POPUP | WS_CLIPCHILDREN | WS_CLIPSIBLINGS);
			SetWindowLong(m_windowHandle, GWL_EXSTYLE, WS_EX_APPWINDOW);

			// Resize the window to screen size
			SetWindowPos(m_windowHandle,
			HWND_TOP,
							0, 0, videoMode.getWidth(), videoMode.getHeight(),
							SWP_FRAMECHANGED);
			ShowWindow(m_windowHandle, SW_SHOW);

		}

		void WindowImplWin32::setDimensions(const Vector2i& dimensions) {

			// SetWindowPos wants the total size of the window (including title bar and borders),
			// so we have to compute it
			RECT rectangle = {
			0,
			0,
			static_cast<long>(dimensions.x),
			static_cast<long>(dimensions.y) };

			if(!AdjustWindowRect(&rectangle,
			GetWindowLong(m_windowHandle, GWL_STYLE),
									false))
				burnErr("Call to AdjustWindowRect failed!");

			int width = rectangle.right - rectangle.left;
			int height = rectangle.bottom - rectangle.top;

			if(!SetWindowPos(m_windowHandle,
			NULL,
								0, 0, width, height,
								SWP_NOMOVE | SWP_NOZORDER))
				burnErr("Call to SetWindowPos failed!");

		}

		void WindowImplWin32::setTitle(const std::string& title) {

			if(!SetWindowText(m_windowHandle, title.c_str()))
				burnErr("Call to SetWindowText failed!");

		}

		WindowHandle WindowImplWin32::getWindowHandle() const {
			return m_windowHandle;
		}

		void WindowImplWin32::processEvents() {

			MSG message;
			while(PeekMessage(&message, NULL, 0, 0, PM_REMOVE)){
				TranslateMessage(&message);
				DispatchMessage(&message);
			}

		}

		void WindowImplWin32::cleanup() {

			m_windowHandle = NULL;

		}

		void WindowImplWin32::registerWindowClass() {

			WNDCLASSEX windowClass;
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = CS_OWNDC;
			windowClass.lpfnWndProc = &WindowImplWin32::globalWindowProcess;
			windowClass.cbClsExtra = 0;
			windowClass.cbWndExtra = 0;
			windowClass.hInstance = GetModuleHandle(NULL);
			windowClass.hIcon = LoadIcon(GetModuleHandle(NULL), IDI_WINLOGO);
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
			windowClass.lpszMenuName = NULL;
			windowClass.lpszClassName = className;
			windowClass.hIconSm = LoadIcon( GetModuleHandle(NULL),
			IDI_WINLOGO);

			if(!RegisterClassEx(&windowClass)){
				burnErr("Failed to register Win32 class!");
			}

		}

		void WindowImplWin32::processWin32Event(UINT msg,
												WPARAM wParam,
												LPARAM) {

			// Don't handle anything before window is created
			if(m_windowHandle == NULL)
				return;

			Event event;

			switch (msg) {
				case WM_DESTROY:
					// Window destroyed (terminated or closed)
					cleanup();
					break;
				case WM_CLOSE:
					// Window requested to be closed
					event.setType(Event::CLOSED);
					break;
				case WM_KEYDOWN:
				case WM_SYSKEYDOWN:
					// Some key was pressed
					event.setType(Event::KEY_PRESSED);
					event.setKey(toBurngineKey(wParam));
					break;
				case WM_KEYUP:
				case WM_SYSKEYUP:
					// Some key was released
					event.setType(Event::KEY_RELEASED);
					event.setKey(toBurngineKey(wParam));
					break;
				case WM_CHAR:
					event.setType(Event::CHARACTER);
					event.setCharacter((char)wParam);
					break;
				default:
					// Unknown or unhandled event
					event.setType(Event::UNKNOWN_EVENT);
					break;
			}

			// Pass event upwards to the user
			pushEvent(event);
		}

		LRESULT CALLBACK WindowImplWin32::globalWindowProcess(	HWND hWnd,
																UINT msg,
																WPARAM wParam,
																LPARAM lParam) {

			// Pass CREATE events directly to DefWindowProc
			if(msg == WM_NCCREATE || msg == WM_CREATE)
				return DefWindowProc(hWnd, msg, wParam, lParam);

			// If window has been mapped then process the win32 event
			if(windowMap.find(hWnd) != windowMap.end())
				windowMap[hWnd]->processWin32Event(msg, wParam, lParam);

			// We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
			if(msg == WM_CLOSE)
				return 0;

			// Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
			if((msg == WM_SYSCOMMAND) && (wParam == SC_KEYMENU))
				return 0;

			return DefWindowProc(hWnd, msg, wParam, lParam);
		}

		Keyboard::Key WindowImplWin32::toBurngineKey(const WPARAM& win32Key) {

			Keyboard::Key key = Keyboard::UNKNOWN_KEY;

			// Get the letters
			for(int i = 0; i != 26; ++i){
				if(win32Key == (WPARAM)('A' + i)){
					key = (Keyboard::Key)(Keyboard::A + i);
				}
			}

			// Function key
			for(int i = 0; i != 12; ++i){
				if(win32Key == (WPARAM)(VK_F1 + i)){
					key = (Keyboard::Key)(Keyboard::F1 + i);
				}
			}

			// Numpad
			for(int i = 0; i != 10; ++i){
				if(win32Key == (WPARAM)(VK_NUMPAD0 + i)){
					key = (Keyboard::Key)(Keyboard::NUM_0 + i);
				}
			}

			// Other keys
			switch (win32Key) {
				case VK_ESCAPE:
					key = Keyboard::ESCAPE;
					break;
				case VK_DECIMAL:
					key = Keyboard::COMMA;
					break;
				case VK_SEPARATOR:
					key = Keyboard::PERIOD;
					break;
				case VK_PRINT:
					key = Keyboard::PRINT;
					break;
				case VK_PAUSE:
					key = Keyboard::PAUSE;
					break;
				case VK_CRSEL:
					key = Keyboard::CIRCUM;
					break;
				case VK_ATTN:
					key = Keyboard::ACCENT;
					break;
				case VK_TAB:
					key = Keyboard::TABULATOR;
					break;
				case VK_RETURN:
					key = Keyboard::RETURN;
					break;
				case VK_SHIFT:
					key = Keyboard::SHIFT;
					break;
				case VK_BACK:
					key = Keyboard::BACKSPACE;
					break;
				case VK_CAPITAL:
					key = Keyboard::CAPSLOCK;
					break;
				case VK_CONTROL:
					key = Keyboard::CONTROL;
					break;
				case VK_CONVERT:
					key = Keyboard::ALT;
					break;
				case VK_SPACE:
					key = Keyboard::SPACE;
					break;
				case VK_INSERT:
					key = Keyboard::INSERT;
					break;
				case VK_HOME:
					key = Keyboard::HOME;
					break;
				case VK_DELETE:
					key = Keyboard::DEL;
					break;
				case VK_END:
					key = Keyboard::END;
					break;
				case VK_PRIOR:
					key = Keyboard::PAGE_UP;
					break;
				case VK_NEXT:
					key = Keyboard::PAGE_DOWN;
					break;
				case VK_UP:
					key = Keyboard::ARROW_UP;
					break;
				case VK_LEFT:
					key = Keyboard::ARROW_LEFT;
					break;
				case VK_DOWN:
					key = Keyboard::ARROW_DOWN;
					break;
				case VK_RIGHT:
					key = Keyboard::ARROW_RIGHT;
					break;
				case VK_DIVIDE:
					key = Keyboard::DIVIDE;
					break;
				case VK_MULTIPLY:
					key = Keyboard::MULTIPLY;
					break;
				case VK_SUBTRACT:
					key = Keyboard::SUBTRACT;
					break;
				case VK_ADD:
					key = Keyboard::ADD;
					break;
				case '0':
					key = Keyboard::_0;
					break;
				case '1':
					key = Keyboard::_1;
					break;
				case '2':
					key = Keyboard::_2;
					break;
				case '3':
					key = Keyboard::_3;
					break;
				case '4':
					key = Keyboard::_4;
					break;
				case '5':
					key = Keyboard::_5;
					break;
				case '6':
					key = Keyboard::_6;
					break;
				case '7':
					key = Keyboard::_7;
					break;
				case '8':
					key = Keyboard::_8;
					break;
				case '9':
					key = Keyboard::_9;
					break;
			}

			return key;

		}

	}
} /* namespace burn */
