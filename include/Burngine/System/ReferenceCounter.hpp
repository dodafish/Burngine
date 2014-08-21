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

#ifndef REFERENCECOUNTER_HPP_
#define REFERENCECOUNTER_HPP_

#include <Burngine/Export.hpp>

namespace burn {

/**
 * @brief Keeps track of copy and assignment operations and counts the
 * number of references
 */
class BURNGINE_API_EXPORT ReferenceCounter {
public:

	/**
	 * @brief default constructor. Initializes the counter
	 */
	ReferenceCounter();

	/**
	 * @brief Copy-constructor. Increments the counter
	 */
	ReferenceCounter(const ReferenceCounter& other);

	/**
	 * @brief Assignment-operator. Decreases this counter and increments
	 * the other counter.
	 */
	ReferenceCounter& operator=(const ReferenceCounter& other);

protected:

	/**
	 * @brief Is this the last reference?
	 *
	 * @return True if this is the last reference
	 */
	bool isLastReference() const;

	/**
	 * @brief Called when the last reference gets destroyed
	 */
	virtual void onReferenceLoss();

private:

	Uint32* m_counter;    ///< Reference counter

};

} /* namespace burn */

#endif /* REFERENCECOUNTER_HPP_ */
