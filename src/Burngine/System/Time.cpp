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

#include <Burngine/System/Time.hpp>

namespace burn {

	Time::Time() :
	m_duration(0) {

	}

	Time::Time(	const std::chrono::high_resolution_clock::time_point& start,
				const std::chrono::high_resolution_clock::time_point& end) :
	m_duration(std::chrono::duration_cast<std::chrono::microseconds>(end - start)) {

	}

	double Time::asSeconds() const {
		return m_duration.count() / 1000000.0;
	}

	Int64 Time::asMilliseconds() const {
		return m_duration.count() / 1000;
	}

	long long Time::asMicroseconds() const {
		return m_duration.count();
	}

	void Time::operator+=(const Time& other) {

		m_duration += other.m_duration;

	}

} /* namespace burn */
