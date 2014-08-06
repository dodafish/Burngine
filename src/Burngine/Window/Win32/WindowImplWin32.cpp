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

#include <iostream>
#include <string>
#include <tchar.h>
#include <map>

namespace {

unsigned int windowCount = 0;
LPCTSTR className = TEXT("Burngine_WinClass");

std::map<HWND, burn::priv::WindowImplWin32*> windowMap;

}

namespace burn {
namespace priv {

WindowImplWin32::WindowImplWin32(	const VideoMode& videoMode,
									const std::string& title,
									const Window::Style& style) :
m_windowHandle(NULL) {

	// Register window class if this is the first window
	if(windowCount == 0)
		registerWindowClass();

	// Set a titlebar as minimum for style
	DWORD windowStyle = WS_SYSMENU | WS_CAPTION | WS_MINIMIZEBOX;
	if(style != Window::FIXED_SIZE){
		windowStyle |= WS_MAXIMIZEBOX;    // Maximize button
		windowStyle |= WS_SIZEBOX;    // Sizeable border
	}

	m_windowHandle = CreateWindow(className, title.c_str(),
	windowStyle,
	CW_USEDEFAULT, CW_USEDEFAULT, videoMode.getWidth(), videoMode.getHeight(),
	NULL,
	NULL,
	GetModuleHandle(NULL),
	NULL);

	if(!m_windowHandle){
		std::cerr << "Failed creating Win32 window!\n";
		std::cerr << "Error: " << GetLastError() << "\n";
		creationFail();
		return;
	}

	ShowWindow(m_windowHandle, SW_SHOW);
	UpdateWindow(m_windowHandle);

	windowCount++;

	// Map the window for event process call
	windowMap.insert(std::pair<HWND, WindowImplWin32*>(m_windowHandle, this));

	std::cout << "Created Win32 window with handle: " << m_windowHandle << "\n";

}

WindowImplWin32::~WindowImplWin32() {
	cleanup();
}

void WindowImplWin32::setDimensions(const Vector2i& dimensions) {

	// SetWindowPos wants the total size of the window (including title bar and borders),
	// so we have to compute it
	RECT rectangle = {
	0,
	0,
	static_cast<long>(dimensions.x),
	static_cast<long>(dimensions.y) };
	AdjustWindowRect(&rectangle, GetWindowLong(m_windowHandle, GWL_STYLE), false);
	int width = rectangle.right - rectangle.left;
	int height = rectangle.bottom - rectangle.top;

	SetWindowPos(m_windowHandle, NULL, 0, 0, width, height, SWP_NOMOVE | SWP_NOZORDER);

}

void WindowImplWin32::setTitle(const std::string& title) {

	SetWindowText(m_windowHandle, title.c_str());

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

	if(m_windowHandle)
		DestroyWindow(m_windowHandle);

	// Last window? Cleanup!
	if(windowCount == 1){
		UnregisterClass(className, GetModuleHandle(NULL));
	}

	m_windowHandle = NULL;

	windowCount--;

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
	windowClass.hIconSm = LoadIcon(GetModuleHandle(NULL), IDI_WINLOGO);

	if(!RegisterClassEx(&windowClass)){
		std::cerr << "Failed to register window class!\n";
		std::cerr << "Error: " << GetLastError() << "\n";
	}

}

void WindowImplWin32::processWin32Event(UINT msg,
										WPARAM wParam,
										LPARAM lParam) {

	// Don't handle anything before window is created
	if(m_windowHandle == NULL)
		return;

	// TEMPORARY: To make wParam and lParam used:
	unsigned int a, b;
	a = wParam;
	b = lParam;
	a += b;

	Event event;

	switch (msg) {
		case WM_DESTROY:
			// Window destroyed! (Not just closed)
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
			/*
			 * Missing the numbers above the letters
			 */
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
			/*
			 * Missing PageUp and PageDown
			 */
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
	}

	return key;

}

}
} /* namespace burn */
