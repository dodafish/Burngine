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

#include <Burngine/System/ReferenceCounter.hpp>

namespace burn {

ReferenceCounter::ReferenceCounter() :
m_counter(new Uint32(1)) {

}

ReferenceCounter::ReferenceCounter(const ReferenceCounter& other) :
m_counter(other.m_counter) {

	++(*m_counter);

}

ReferenceCounter& ReferenceCounter::operator=(const ReferenceCounter& other) {

	// Avoid self assignment
	if(this == &other)
		return *this;

	if(--(*m_counter) == 0){
		onReferenceLoss();
		delete m_counter;
	}

	m_counter = other.m_counter;
	++(*m_counter);

	return *this;
}

bool ReferenceCounter::isLastReference() const {
	return *m_counter == 1;
}

void ReferenceCounter::onReferenceLoss() {
	// Has to be implemented by derived classes
}

}
/* namespace burn */
