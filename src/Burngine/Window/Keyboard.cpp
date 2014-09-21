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

#include <Burngine/Window/Keyboard.hpp>

namespace {

	bool areKeystatesInitialized = false;

	void resetKeyStates() {
		for(int i = 0; i != burn::Keyboard::COUNT; ++i)
			burn::Keyboard::setKeyPressed(	(burn::Keyboard::Key)(burn::Keyboard::UNKNOWN_KEY
											+ i),
											false);
		areKeystatesInitialized = true;
	}

}

namespace burn {

	bool Keyboard::m_keys[];

	bool Keyboard::isKeyPressed(const Key& key) {
		if(key == COUNT)
			return false;
		if(!areKeystatesInitialized)
			resetKeyStates();
		return m_keys[key];
	}

	void Keyboard::setKeyPressed(	const Key& key,
									bool pressed) {
		if(key == COUNT)
			return;
		if(!areKeystatesInitialized)
			resetKeyStates();
		m_keys[key] = pressed;
	}

} /* namespace burn */
