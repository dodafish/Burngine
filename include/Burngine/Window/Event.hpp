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
		enum Type {
			UNKNOWN_EVENT = 0,    ///< Unkown window event (or not handled)
			CLOSED,    ///< Window shall close (e.g. Alt+F4)
			KEY_PRESSED,    ///< A key was pressed (data: getKey())
			KEY_RELEASED,    ///< A key was released (data: getKey())
			CHARACTER    ///< Character has been entered
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

		/**
		 * @brief Set the entered character
		 *
		 * @param Entered character
		 */
		void setCharacter(const char& character);

		/**
		 * @brief Get the entered character
		 *
		 * @return Entered character
		 */
		const char& getCharacter() const;

	private:

		Type m_type;    ///< Event type
		Keyboard::Key m_key;    ///< The pressed/released key
		char m_character;    ///< Entered character

	};

} /* namespace burn */

#endif /* EVENT_HPP_ */
