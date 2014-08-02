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

#include <Burngine/Window/Unix/WindowImplX11.hpp>

#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <iostream>
#include <string>

namespace burn {
namespace priv {

WindowImplX11::WindowImplX11() {

	if ((m_display = XOpenDisplay(NULL)) == NULL) {
		std::cout << ("Unable to open display\n");
		return;
	}

	m_window = XCreateSimpleWindow(m_display, DefaultRootWindow(m_display), 0,
			0, 400, 300, BlackPixel(m_display, DefaultScreen(m_display)), 0,
			WhitePixel(m_display, DefaultScreen(m_display)));

	XStoreName(m_display, m_window, "Burngine Window - Unix X11-Window");

	std::string resName("WindowName_Burn");
	std::string resClass("WindowClass_Burn");

	m_ClassHint.res_name = &resName[0];
	m_ClassHint.res_class = &resClass[0];
	XSetClassHint(m_display, m_window, &m_ClassHint);

	m_deleteAtom = XInternAtom(m_display, "WM_DELETE_WINDOW", True);

	if (m_deleteAtom)
		XSetWMProtocols(m_display, m_window, &m_deleteAtom, 1);

	XMapWindow(m_display, m_window);

	XFlush(m_display);

	XSelectInput(m_display, m_window, KeyPressMask | ButtonPressMask);

}

void WindowImplX11::processEvents() {

	XEvent event;

	while (XPending(m_display) > 0) {
		XNextEvent(m_display, &event);
		switch (event.type) {
		case ClientMessage:
			if (event.xclient.data.l[0] == (long) m_deleteAtom)
				pushEvent(Event(Event::CLOSED));
			break;
		case KeyPress:
			// Check all letters (A to Z)
			for (int i = 0; i != 26; ++i)
				if (event.xkey.keycode
						== XKeysymToKeycode(m_display, XK_A + i)) {
					Event keyEvent(Event::KEY_PRESSED);
					keyEvent.setKey((Keyboard::Key) (Keyboard::A + i));
					pushEvent(keyEvent);
				}
			break;
		case KeyRelease:
			// Check all letters (A to Z)
			for (int i = 0; i != 26; ++i)
				if (event.xkey.keycode
						== XKeysymToKeycode(m_display, XK_A + i)) {
					Event keyEvent(Event::KEY_RELEASED);
					keyEvent.setKey((Keyboard::Key) (Keyboard::A + i));
					pushEvent(keyEvent);
				}
			break;
		default:
			break;
		}
	}

}

}
} /* namespace burn */
