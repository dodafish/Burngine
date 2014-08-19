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

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/Time.hpp>

#include <chrono>

namespace burn {

/**
 * @brief Measures time precisly
 */
class BURNGINE_API_EXPORT Clock {
public:

	/**
	 * @brief Sets the elapsed time to 0 and starts the clock
	 */
	Clock();

	/**
	 * @brief Reset time measurement
	 */
	void reset();

	/**
	 * @brief Continue time measurement
	 */
	void start();

	/**
	 * @brief Stop measuring
	 */
	void stop();

	/**
	 * @brief Get measured time
	 */
	const Time& getElapsedTime();

private:

	std::chrono::high_resolution_clock::time_point m_start;    ///< Start point
	std::chrono::high_resolution_clock::time_point m_end;    ///< End point

	bool m_isRunning; ///< Is the clock started?

	Time m_time;    ///< Time for current measuring
	Time m_elapsed; ///< Time for measurement fetching. Temporary.

};

} /* namespace burn */

#endif /* TIMER_HPP_ */
