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

#ifndef WINDOWIMPL_HPP_
#define WINDOWIMPL_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/Event.hpp>
#include <Burngine/Window/VideoMode.hpp>
#include <Burngine/Window/Window.hpp>
#include <Burngine/Window/WindowHandle.hpp>
#include <queue>
#include <string>

namespace burn {
namespace priv {

/**
 * @brief Platform specific window implementation
 */
class WindowImpl {
public:

	/**
	 * @brief Creates a platform specific window.
	 *
	 * - WindowImplX11 on Unix (X11 window)
	 *
	 * @param videoMode Desired video mode
	 * @param title Desired title
	 *
	 * @return Platform specific window implementation
	 */
	static WindowImpl* create(	const VideoMode& videoMode,
								const std::string& title,
								const Window::Style& style);

public:

	/**
	 * @brief Virtual destructor.
	 * Calls D-Tor of derived class to process cleanup
	 */
	virtual ~WindowImpl();

	/**
	 * @brief Check if window has been created successfully
	 *
	 * @return True on success
	 */
	bool creationSucceeded() const;

	/**
	 * @brief Get the next/latest window event and check for
	 * new ones
	 *
	 * @param event Latest event will be assigned to this
	 *
	 * @return True if there was an event
	 */
	bool popEvent(Event& event);

	/**
	 * @brief Apply new dimensions on window.
	 * This is handled individually per platform.
	 *
	 * @param dimensions Desired dimensions
	 */
	virtual void setDimensions(const Vector2i& dimensions) = 0;

	/**
	 * @brief Apply new title on window.
	 * This is handled individually per platform.
	 *
	 * @param title Desired title
	 */
	virtual void setTitle(const std::string& title) = 0;

	virtual WindowHandle getWindowHandle() const = 0;

protected:

	/**
	 * @brief Default constructor sets m_creationSucceeded to true.
	 * Derived classes call creationFail() to set it to false.
	 * This way the user can check the creation with
	 * creationSucceeded()
	 */
	WindowImpl();

	/**
	 * @brief Tell that window creation failed.
	 * Used by derived classes.
	 */
	void creationFail();

	/**
	 * @brief Add a new event to the queue.
	 * Used by derived classes.
	 */
	void pushEvent(const Event& event);

	/**
	 * @brief Process all window events and add them to queue.
	 * This is handled individually per platform.
	 */
	virtual void processEvents() = 0;

private:

	bool m_creationSucceeded;    ///< Turns false if window creation failed
	std::queue<Event> m_events;    ///< All events to be polled

};

} /* namespace priv */
} /* namespace burn */

#endif /* WINDOWIMPL_HPP_ */
