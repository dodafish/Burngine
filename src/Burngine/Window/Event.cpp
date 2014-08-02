/*
 * Event.cpp
 *
 *  Created on: 02.08.2014
 *      Author: dominik
 */

#include <Burngine/Window/Event.hpp>

namespace burn {

Event::Event(const Type& type) :
		m_type(type), m_key(Keyboard::UNKNOWN_KEY) {

}

const Event::Type& Event::getType() const {
	return m_type;
}

Event::Event() :
		m_type(UNKNOWN_EVENT), m_key(Keyboard::UNKNOWN_KEY) {

}

void Event::setKey(const Keyboard::Key& key) {
	m_key = key;
}

const Keyboard::Key& Event::getKey() const {
	return m_key;
}

} /* namespace burn */
