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

#ifndef THREAD_HPP_
#define THREAD_HPP_

#include <Burngine/Export.hpp>

namespace burn {

	class BURNGINE_API_EXPORT Thread {
	public:

		/**
		 * @brief Prepare a thread that calls function when
		 * it is started
		 *
		 * @param function pointer to a function with argument of type void* and with
		 * return type void
		 * @param arg Argument that is passed to function
		 */
		Thread(	void* (*function)(void*),
				void* arg);

		/**
		 * @brief Releases allocated memory
		 */
		~Thread();

		/**
		 * @brief Start thread execution
		 */
		void launch();

		/**
		 * @brief Wait until thread finished execution
		 */
		void join();

	private:

		void* m_thread;    ///< pthread_t instance
		void* (*m_function)(void*);    ///< function that is called
		void* m_arg;    ///< argument that is passed to function

	};

} /* namespace burn */

#endif /* THREAD_HPP_ */
