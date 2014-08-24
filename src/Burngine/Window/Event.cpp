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

#include <Burngine/Window/Event.hpp>

namespace burn {

	Event::Event() :
	m_type(UNKNOWN_EVENT),
	m_key(Keyboard::UNKNOWN_KEY),
	m_character(0) {

	}

	Event::Event(const Type& type) :
	m_type(type),
	m_key(Keyboard::UNKNOWN_KEY),
	m_character(0) {

	}

	void Event::setType(const Type& type) {
		m_type = type;
	}

	const Event::Type& Event::getType() const {
		return m_type;
	}

	void Event::setKey(const Keyboard::Key& key) {
		m_key = key;
	}

	const Keyboard::Key& Event::getKey() const {
		return m_key;
	}

	void Event::setCharacter(const char& character) {
		m_character = character;
	}

	const char& Event::getCharacter() const {
		return m_character;
	}

} /* namespace burn */
