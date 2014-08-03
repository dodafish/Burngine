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

#ifndef VECTOR2_HPP_
#define VECTOR2_HPP_

#include <Burngine/Export.hpp>

namespace burn {

/**
 * @brief Set of two variables of same type.
 * Used with elementary variable types like int or float.
 */
template<typename T>
class BURNGINE_API_EXPORT Vector2 {
public:

	/**
	 * @brief Default constructor
	 * Same as Vector2<T>(0, 0);
	 */
	Vector2();

	/**
	 * @brief Create a vector with defined initial values
	 *
	 * @param first Value for x
	 * @param second Value for y
	 */
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
