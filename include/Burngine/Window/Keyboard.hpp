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

#ifndef KEYBOARD_HPP_
#define KEYBOARD_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/NonInstancable.hpp>

namespace burn {

/**
 * @brief Handles Keycodes
 * Static-only class
 */
class BURNGINE_API_EXPORT Keyboard: public NonInstancable {
public:

	/**
	 * @brief All key(codes)
	 */
	enum Key {
		UNKNOWN_KEY = 0, ///< An unknown key
		A, ///< The 'A' key
		B, ///< The 'B' key
		C, ///< The 'C' key
		D, ///< The 'D' key
		E, ///< The 'E' key
		F, ///< The 'F' key
		G, ///< The 'G' key
		H, ///< The 'H' key
		I, ///< The 'I' key
		J, ///< The 'J' key
		K, ///< The 'K' key
		L, ///< The 'L' key
		M, ///< The 'M' key
		N, ///< The 'N' key
		O, ///< The 'O' key
		P, ///< The 'P' key
		Q, ///< The 'Q' key
		R, ///< The 'R' key
		S, ///< The 'S' key
		T, ///< The 'T' key
		U, ///< The 'U' key
		V, ///< The 'V' key
		W, ///< The 'W' key
		X, ///< The 'X' key
		Y, ///< The 'Y' key
		Z, ///< The 'Z' key
		COMMA,
		PERIOD,
		ESCAPE,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		PRINT,
		PAUSE,
		CIRCUM,
		_1,
		_2,
		_3,
		_4,
		_5,
		_6,
		_7,
		_8,
		_9,
		_0,
		ACCENT,
		TABULATOR,
		RETURN, ///< The '<_|' key
		SHIFT, ///< The '/\' key
		BACKSPACE, ///< The '<===' key
		CAPSLOCK,
		CONTROL,
		ALT,
		SPACE,
		INSERT,
		HOME,
		DEL,
		END,
		PAGE_UP,
		PAGE_DOWN,
		ARROW_UP,
		ARROW_LEFT,
		ARROW_DOWN,
		ARROW_RIGHT,
		NUM_0,
		NUM_1,
		NUM_2,
		NUM_3,
		NUM_4,
		NUM_5,
		NUM_6,
		NUM_7,
		NUM_8,
		NUM_9,
		DIVIDE,
		MULTIPLY,
		SUBTRACT,
		ADD
	};

};

} /* namespace burn */

#endif /* KEYBOARD_HPP_ */
