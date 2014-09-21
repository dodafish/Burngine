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

		// Letters
		for(int i = 0; i != 26; ++i){
			if(key == burn::Keyboard::A + i){
				return (int)'A' + i;
			}
		}

		// Function keys
		for(int i = 0; i != 12; ++i){
			if(key == burn::Keyboard::F1 + i)
				return VK_F1 + i;
		}

		// Numpad
		for(int i = 0; i != 10; ++i){
			if(key == burn::Keyboard::NUM_0 + i)
				return VK_NUMPAD0 + i;
		}

		// Other keys
		switch (key) {
			case burn::Keyboard::ESCAPE:
				return VK_ESCAPE;
				break;
			case burn::Keyboard::COMMA:
				return VK_DECIMAL;
				break;
			case burn::Keyboard::PERIOD:
				return VK_SEPARATOR;
				break;
			case burn::Keyboard::PRINT:
				return VK_PRINT;
				break;
			case burn::Keyboard::CIRCUM:
				return VK_CRSEL;
				break;
			case burn::Keyboard::TABULATOR:
				return VK_TAB;
				break;
			case burn::Keyboard::RETURN:
				return VK_RETURN;
				break;
			case burn::Keyboard::SHIFT:
				return VK_SHIFT;
				break;
			case burn::Keyboard::BACKSPACE:
				return VK_BACK;
				break;
			case burn::Keyboard::CAPSLOCK:
				return VK_CAPITAL;
				break;
			case burn::Keyboard::CONTROL:
				return VK_CONTROL;
				break;
			case burn::Keyboard::ALT:
				return VK_CONVERT;
				break;
			case burn::Keyboard::SPACE:
				return VK_SPACE;
				break;
			case burn::Keyboard::INSERT:
				return VK_INSERT;
				break;
			case burn::Keyboard::HOME:
				return VK_HOME;
				break;
			case burn::Keyboard::DEL:
				return VK_DELETE;
				break;
			case burn::Keyboard::END:
				return VK_END;
				break;
			case burn::Keyboard::PAGE_UP:
				return VK_PRIOR;
				break;
			case burn::Keyboard::PAGE_DOWN:
				return VK_NEXT;
				break;
			case burn::Keyboard::ARROW_UP:
				return VK_UP;
				break;
			case burn::Keyboard::ARROW_DOWN:
				return VK_DOWN;
				break;
			case burn::Keyboard::ARROW_LEFT:
				return VK_LEFT;
				break;
			case burn::Keyboard::ARROW_RIGHT:
				return VK_RIGHT;
				break;
			case burn::Keyboard::DIVIDE:
				return VK_DIVIDE;
				break;
			case burn::Keyboard::MULTIPLY:
				return VK_MULTIPLY;
				break;
			case burn::Keyboard::SUBTRACT:
				return VK_SUBTRACT;
				break;
			case burn::Keyboard::ADD:
				return VK_ADD;
				break;
			case burn::Keyboard::_0:
				return (int)'0';
				break;
			case burn::Keyboard::_1:
				return (int)'1';
				break;
			case burn::Keyboard::_2:
				return (int)'2';
				break;
			case burn::Keyboard::_3:
				return (int)'3';
				break;
			case burn::Keyboard::_4:
				return (int)'4';
				break;
			case burn::Keyboard::_5:
				return (int)'5';
				break;
			case burn::Keyboard::_6:
				return (int)'6';
				break;
			case burn::Keyboard::_7:
				return (int)'7';
				break;
			case burn::Keyboard::_8:
				return (int)'8';
				break;
			case burn::Keyboard::_9:
				return (int)'9';
				break;

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
