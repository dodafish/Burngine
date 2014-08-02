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

WindowImplX11::WindowImplX11(const VideoMode& videoMode,
		const std::string& title) {

	// Connect to display server
	if ((m_display = XOpenDisplay(NULL)) == NULL) {
		std::cerr << ("Unable to open display\n");
		return;
	}

	// Create the window
	m_window = XCreateSimpleWindow(m_display, DefaultRootWindow(m_display), 0,
			0, videoMode.getWidth(), videoMode.getHeight(),
			BlackPixel(m_display, DefaultScreen(m_display)), 0,
			WhitePixel(m_display, DefaultScreen(m_display)));

	// Give it a title
	XStoreName(m_display, m_window, title.c_str());

	// Register the window class
	std::string resName(title);
	std::string resClass(title + "_class");
	m_ClassHint.res_name = &resName[0];
	m_ClassHint.res_class = &resClass[0];
	XSetClassHint(m_display, m_window, &m_ClassHint);

	// Link the quit event if possible
	m_deleteAtom = XInternAtom(m_display, "WM_DELETE_WINDOW", True);
	if (m_deleteAtom)
		XSetWMProtocols(m_display, m_window, &m_deleteAtom, 1);
	else
		std::cerr << "Unable to link window's delete atom!\n";

	// Show window
	XMapWindow(m_display, m_window);

	// Update display
	XFlush(m_display);

	// Select event inputs
	XSelectInput(m_display, m_window, KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);

}

WindowImplX11::~WindowImplX11() {

	// Destroy the window and disconnect from display server
	XDestroyWindow(m_display, m_window);
	XCloseDisplay(m_display);

}

void WindowImplX11::setDimensions(const Vector2i& dimensions) {

	// Set new attributes in XWindowChanges
	XWindowChanges changes;
	changes.width = dimensions.x;
	changes.height = dimensions.y;

	// Configure window with new attributes
	XConfigureWindow(m_display, m_window, CWWidth | CWHeight, &changes);

}

void WindowImplX11::setTitle(const std::string& title) {

	// Give window the title
	XStoreName(m_display, m_window, title.c_str());

}

void WindowImplX11::processEvents() {

	// Event object
	XEvent event;

	// Poll all events
	while (XPending(m_display) > 0) {
		XNextEvent(m_display, &event);

		switch (event.type) {
		case ClientMessage: // Window wants to close
			if (event.xclient.data.l[0] == (long) m_deleteAtom)
				pushEvent(Event(Event::CLOSED));
			break;
		case KeyPress: // A key was pressed
			// Check all letters (A to Z)
			for (int i = 0; i != 26; ++i)
				if (event.xkey.keycode
						== XKeysymToKeycode(m_display, XK_A + i)) {
					Event keyEvent(Event::KEY_PRESSED);
					keyEvent.setKey((Keyboard::Key) (Keyboard::A + i));
					pushEvent(keyEvent);
				}
			break;
		case KeyRelease: // A key was released
			// Check all letters (A to Z)
			for (int i = 0; i != 26; ++i)
				if (event.xkey.keycode
						== XKeysymToKeycode(m_display, XK_A + i)) {
					Event keyEvent(Event::KEY_RELEASED);
					keyEvent.setKey((Keyboard::Key) (Keyboard::A + i));
					pushEvent(keyEvent);
				}
			break;
		default: // Unhandled event
			pushEvent(Event(Event::UNKNOWN_EVENT));
			break;
		}
	}

}

}
} /* namespace burn */
