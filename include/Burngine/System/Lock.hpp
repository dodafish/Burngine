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

#ifndef LOCK_HPP_
#define LOCK_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/Mutex.hpp>
#include <Burngine/System/NonCopyable.hpp>

namespace burn {

	/**
	 * @brief Locks a mutex and unlocks it automatically at destruction
	 */
	class BURNGINE_API_EXPORT Lock : public NonCopyable {
	public:

		/**
		 * @brief Try locking the mutex. Thread will wait until it
		 * successfully acquired the mutex.
		 * The destructor takes care of unlock the mutex.
		 */
		Lock(Mutex& mutex);

		/**
		 * @brief Unlocks the mutex
		 */
		~Lock();

	private:
		Mutex& m_mutex; ///< Locked mutex
	};

} /* namespace burn */

#endif /* LOCK_HPP_ */
