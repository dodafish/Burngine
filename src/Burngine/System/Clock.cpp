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

#include <Burngine/System/Clock.hpp>

namespace burn {

Clock::Clock() :
m_isRunning(true) {
	reset();
}

void Clock::reset() {

	m_start = m_end = std::chrono::high_resolution_clock::now();
	m_time = Time(m_start, m_end);

}

void Clock::start() {

	if(!m_isRunning){

		m_start = std::chrono::high_resolution_clock::now();

		m_isRunning = true;
	}

}

void Clock::stop() {

	if(m_isRunning){
		m_end = std::chrono::high_resolution_clock::now();

		m_time += Time(m_start, m_end);
		m_isRunning = false;
	}

}

const Time& Clock::getElapsedTime() const {
	m_elapsed = m_time;
	if(m_isRunning){
		m_elapsed += Time(m_start, m_end);
	}
	return m_elapsed;
}

} /* namespace burn */
