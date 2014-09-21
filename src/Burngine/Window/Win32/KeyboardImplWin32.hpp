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

#ifndef KEYBOARDIMPLWIN32_HPP_
#define KEYBOARDIMPLWIN32_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/Keyboard.hpp>

namespace burn {
	namespace priv {

		class KeyboardImplWin32 : public Keyboard {
		public:

			/**
			 * @brief Get the current state of a key
			 *
			 * @param key Key to get the state of
			 *
			 * @return True if key is pressed. False otherwise
			 */
			static bool isKeyPressed(const Key& key);

		};

	} /* namespace priv */
} /* namespace burn */

#endif /* KEYBOARDIMPLWIN32_HPP_ */
