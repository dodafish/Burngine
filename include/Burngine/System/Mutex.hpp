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

#ifndef MUTEX_HPP_
#define MUTEX_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/NonCopyable.hpp>

namespace burn {

	/**
	 * @brief Used as lock for threads to prevent concurrent data access
	 */
	class BURNGINE_API_EXPORT Mutex : public NonCopyable {
	public:

		/**
		 * @brief Initializes the mutex
		 */
		Mutex();

		/**
		 * @brief Try locking the mutex. Thread won't continue until
		 * it successfully locked it.
		 * Don't forget to unlock()!
		 */
		void lock();

		/**
		 * @brief Unlock the mutex and make it lockable for other threads
		 */
		void unlock();

	private:
		void* m_mutex; ///< void* for hiding pthread api
	};

} /* namespace burn */

#endif /* MUTEX_HPP_ */
