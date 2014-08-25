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

#ifndef TIME_HPP_
#define TIME_HPP_

#include <Burngine/Export.hpp>
#include <chrono>

namespace burn {

	/**
	 * @brief Saves elapsed time between two time points
	 */
	class BURNGINE_API_EXPORT Time {
	public:

		/**
		 * @brief Default ctor. Sets duration to zero.
		 */
		Time();

		/**
		 * @brief Save duration depending on two time points
		 */
		Time(	const std::chrono::high_resolution_clock::time_point& start,
				const std::chrono::high_resolution_clock::time_point& end);

		/**
		 * @brief Get time as seconds
		 */
		double asSeconds() const;

		/**
		 * @brief Get time as milliseconds
		 */
		Int64 asMilliseconds() const;

		/**
		 * @brief Get time as microseconds
		 */
		long long asMicroseconds() const;

		void operator+=(const Time& other);

	private:

		std::chrono::microseconds m_duration;    ///< Saved duration as microseconds

	};

} /* namespace burn */

#endif /* TIME_HPP_ */
