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

#include <Burngine/Window/Win32/KeyboardImplWin32.hpp>
#include <windows.h>

namespace {

	int toWin32Key(const burn::Keyboard::Key& key) {

		for(int i = 0; i != 26; ++i){
			if(key == burn::Keyboard::A + i){
				return (int)'A' + i;
			}
		}

		return -1;
	}

}

namespace burn {
	namespace priv {

		bool KeyboardImplWin32::isKeyPressed(const Key& key) {

			int w32key = toWin32Key(key);
			if(w32key == -1)
				return false;

			short result = GetAsyncKeyState(w32key);

			if((result & 0x0000) || (result & 0x0001))
				return false;
			if((result & 0x8000) || (result & 0x8001))
				return true;

			return false;
		}

	}
} /* namespace burn */
