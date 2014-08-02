/*
 * Event.hpp
 *
 *  Created on: 02.08.2014
 *      Author: dominik
 */

#ifndef EVENT_HPP_
#define EVENT_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/Keyboard.hpp>

namespace burn {

/**
 * @brief Handles window events
 */
class BURNGINE_API_EXPORT Event {
public:

	/**
	 * @brief All event types
	 */
	enum Type{
		UNKNOWN_EVENT = 0, ///< Unkown window event (or not handled)
		CLOSED, ///< Window shall close (e.g. Alt+F4)
		KEY_PRESSED, ///< A key was pressed (data: getKey())
		KEY_RELEASED ///< A key was released (data: getKey())
	};

	/**
	 * @brief Default constructor.
	 * Event type is by default UNKNOWN
	 */
	Event();

	/**
	 * @brief Create Event with a fixed type.
	 *
	 * @param type Event's type
	 */
	Event(const Type& type);

	/**
	 * @brief Set event's type (CLOSED, KEY_PRESSED, etc.)
	 *
	 * @param Event type
	 */
	void setType(const Type& type);

	/**
	 * @brief Get the type of the event
	 *
	 * @return The event type. (CLOSED, KEY_PRESSED, etc.)
	 */
	const Type& getType() const;

	/**
	 * @brief Set the key in the event
	 * Used internally to send events
	 *
	 * @param key The key for the event
	 */
	void setKey(const Keyboard::Key& key);

	/**
	 * @brief Get the event's key
	 *
	 * @return The event's key. (Keyboard::A, etc.)
	 */
	const Keyboard::Key& getKey() const;

private:

	Type m_type; ///< Event type
	Keyboard::Key m_key; ///< The pressed/released key

};

} /* namespace burn */

#endif /* EVENT_HPP_ */
