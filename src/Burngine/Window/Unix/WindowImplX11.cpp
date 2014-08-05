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

#include <Burngine/Window/Unix/WindowImplX11.hpp>

#include <X11/keysym.h>
#include <X11/keysymdef.h>
#include <iostream>
#include <string>

namespace burn {
namespace priv {

WindowImplX11::WindowImplX11(const VideoMode& videoMode,
		const std::string& title, const Window::Style& style) {

	// Connect to display server
	if ((m_display = XOpenDisplay(NULL)) == NULL) {
		std::cerr << ("Unable to open display\n");
		creationFail();
		return;
	}

	// Create the window
	m_window = XCreateSimpleWindow(m_display, DefaultRootWindow(m_display), 0,
			0, videoMode.getWidth(), videoMode.getHeight(),
			BlackPixel(m_display, DefaultScreen(m_display)), 0,
			WhitePixel(m_display, DefaultScreen(m_display)));

	// Setup style.
	Atom WMHintsAtom = XInternAtom(m_display, "_MOTIF_WM_HINTS", false);
	if (WMHintsAtom) {
		static const unsigned long MWM_HINTS_FUNCTIONS = 1 << 0;
		static const unsigned long MWM_HINTS_DECORATIONS = 1 << 1;

		//static const unsigned long MWM_DECOR_ALL = 1 << 0;
		static const unsigned long MWM_DECOR_BORDER = 1 << 1;
		static const unsigned long MWM_DECOR_RESIZEH = 1 << 2;
		static const unsigned long MWM_DECOR_TITLE = 1 << 3;
		static const unsigned long MWM_DECOR_MENU = 1 << 4;
		static const unsigned long MWM_DECOR_MINIMIZE = 1 << 5;
		static const unsigned long MWM_DECOR_MAXIMIZE = 1 << 6;

		//static const unsigned long MWM_FUNC_ALL = 1 << 0;
		static const unsigned long MWM_FUNC_RESIZE = 1 << 1;
		static const unsigned long MWM_FUNC_MOVE = 1 << 2;
		static const unsigned long MWM_FUNC_MINIMIZE = 1 << 3;
		static const unsigned long MWM_FUNC_MAXIMIZE = 1 << 4;
		static const unsigned long MWM_FUNC_CLOSE = 1 << 5;

		struct WMHints {
			unsigned long flags;
			unsigned long functions;
			unsigned long decorations;
			long inputMode;
			unsigned long state;
		};

		WMHints hints;
		hints.flags = MWM_HINTS_FUNCTIONS | MWM_HINTS_DECORATIONS;
		hints.decorations = 0;
		hints.functions = 0;


			hints.decorations |= MWM_DECOR_BORDER | MWM_DECOR_TITLE
					| MWM_DECOR_MINIMIZE | MWM_DECOR_MENU;
			hints.functions |= MWM_FUNC_MOVE | MWM_FUNC_MINIMIZE | MWM_FUNC_CLOSE;

		if (style != Window::FIXED_SIZE) {
			hints.decorations |= MWM_DECOR_MAXIMIZE | MWM_DECOR_RESIZEH;
			hints.functions |= MWM_FUNC_MAXIMIZE | MWM_FUNC_RESIZE;
		}

		const unsigned char* ptr =
				reinterpret_cast<const unsigned char*>(&hints);
		XChangeProperty(m_display, m_window, WMHintsAtom, WMHintsAtom, 32,
				PropModeReplace, ptr, 5);
	}

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
	XSelectInput(m_display, m_window,
	KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask);

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
