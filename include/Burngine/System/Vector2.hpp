//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef VECTOR2_HPP_
#define VECTOR2_HPP_

#include <Burngine/Export.hpp>

namespace burn {

template<typename T>
class BURNGINE_API_EXPORT Vector2 {
public:
	Vector2();
	Vector2(const T& first, const T& second);

public:
	T x, y;
};

#include <Burngine/System/Vector2.inl>

typedef Vector2<Int32> Vector2i;
typedef Vector2<Uint32> Vector2ui;
typedef Vector2<float> Vector2f;

} /* namespace burn */

#endif /* VECTOR2_HPP_ */
