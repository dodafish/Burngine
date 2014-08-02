//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#include <Burngine/Window/Win32/WindowImplWin32.hpp>

#include <iostream>
#include <string>
#include <tchar.h>
#include <map>

namespace {

unsigned int windowCount = 0;
std::string className = "Burngine_Window_Class";

std::map<HWND, burn::priv::WindowImplWin32*> windowMap;

}

namespace burn {
namespace priv {

WindowImplWin32::WindowImplWin32(	const VideoMode& videoMode,
									const std::string& title) :
m_windowHandle(NULL) {

	// Register window class if this is the first window
	if(windowCount == 0)
		registerWindowClass();

	m_windowHandle = CreateWindow(
	className.c_str(),
	title.c_str(),
	WS_CAPTION | WS_MINIMIZEBOX,
	CW_USEDEFAULT, CW_USEDEFAULT,
	videoMode.getWidth(), videoMode.getHeight(),
	NULL,
	NULL,
	GetModuleHandle(NULL),
	NULL
	);

	if(m_windowHandle == NULL){
		std::cerr << "Failed creating Win32 window!\n";
		std::cerr << "Error: " << GetLastError() << "\n";
	}

	ShowWindow(m_windowHandle, SW_SHOW);
	UpdateWindow(m_windowHandle);

	windowCount++;

	// Map the window for event process call
	windowMap.insert(std::pair<HWND, WindowImplWin32*>(m_windowHandle, this));

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
		UnregisterClass(className.c_str(), GetModuleHandle(NULL));
	}

	m_windowHandle = NULL;

	windowCount--;

}

void WindowImplWin32::registerWindowClass() {
	WNDCLASSEX windowClass;
	windowClass.style = CS_HREDRAW | CS_VREDRAW;
	windowClass.lpfnWndProc = &WindowImplWin32::globalWindowProcess;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = GetModuleHandle(NULL);
	windowClass.hIcon = LoadIcon(windowClass.hInstance, IDI_QUESTION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = (HBRUSH)(COLOR_MENU + 1);
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = className.c_str();
	RegisterClassEx(&windowClass);
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
			// Send CLOSED event nevertheless
			//event.setType(Event::CLOSED);
			break;
		case WM_CLOSE:
			event.setType(Event::CLOSED);
			break;
		case WM_KEYDOWN:
			event.setType(Event::KEY_PRESSED);
			break;
		case WM_KEYUP:
			event.setType(Event::KEY_RELEASED);
			break;
		default:
			event.setType(Event::UNKNOWN_EVENT);

			break;
	}

	std::cout << "EVENT: " << msg << "\n";

	pushEvent(event);

}

LRESULT CALLBACK WindowImplWin32::globalWindowProcess(	HWND hWnd,
														UINT msg,
														WPARAM wParam,
														LPARAM lParam) {

	std::cout << "GLOBAL_EVENT: " << msg << "\n";

	windowMap[hWnd]->processWin32Event(msg, wParam, lParam);

	// We don't forward the WM_CLOSE message to prevent the OS from automatically destroying the window
	if(msg == WM_CLOSE)
		return 0;

	// Don't forward the menu system command, so that pressing ALT or F10 doesn't steal the focus
	if((msg == WM_SYSCOMMAND) && (wParam == SC_KEYMENU))
		return 0;

	return DefWindowProc(hWnd, msg, wParam, lParam);
}

}
} /* namespace burn */
