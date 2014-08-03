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

#ifndef NONCOPYABLE_HPP_
#define NONCOPYABLE_HPP_

#include <Burngine/Export.hpp>

namespace burn {

/**
 * @brief Denies copying
 */
class BURNGINE_API_EXPORT NonCopyable {
protected:

	/**
	 * @brief Protected default constructor.
	 * Needed to avoid compile error. Does nothing.
	 */
	NonCopyable();

private:

	/**
	 * @brief Hidden copy constructor prevents copying
	 */
	NonCopyable(const NonCopyable& other);

	/**
	 * @brief Hidden assignment operator prevents copying
	 */
	NonCopyable& operator=(const NonCopyable& other);
};

} /* namespace burn */
#endif /* NONCOPYABLE_HPP_ */
