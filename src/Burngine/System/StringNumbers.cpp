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

#include <Burngine/System/StringNumbers.hpp>
#include <stdlib.h>

namespace {

	char digit[10] = {
	'1',
	'2',
	'3',
	'4',
	'5',
	'6',
	'7',
	'8',
	'9',
	'0' };

	char sign[2] = {
	'+',
	'-' };

}

namespace burn {

	StringNumbers::StringNumbers() :
	m_position(0) {

	}

	void StringNumbers::setString(const std::string& s) {
		m_string = s;
		m_position = 0;
	}

	bool StringNumbers::nextInt(int& n) {

		// Out of range?
		if(m_position >= m_string.size()){
			return false;
		}

		size_t pos = findNextNumber();

		// Anything found?
		if(pos == m_string.size()){
			return false;
		}

		// Get the last digit of this number
		std::string number = "";
		number += m_string[pos];
		bool hasWholeNumber = false;
		while(!hasWholeNumber){
			hasWholeNumber = true;

			// Out of range?
			if(++pos >= m_string.size())
				break;

			// Check next position
			for(size_t i = 0; i != 10; ++i){
				if(m_string[pos] == digit[i] || m_string[pos] == '.'){
					number += m_string[pos];
					hasWholeNumber = false;
					break;
				}
			}

		}

		// Done. Save new position and store number
		m_position = pos;
		n = strtol(number.c_str(), 0, 0);

		return true;
	}

	bool StringNumbers::nextFloat(float& f) {
		// Out of range?
		if(m_position >= m_string.size()){
			return false;
		}

		size_t pos = findNextNumber();

		// Anything found?
		if(pos == m_string.size()){
			return false;
		}

		// Get the last digit of this number
		std::string number = "";
		number += m_string[pos];
		bool hasWholeNumber = false;
		while(!hasWholeNumber){
			hasWholeNumber = true;

			// Out of range?
			if(++pos >= m_string.size())
				break;

			// Check next position
			for(size_t i = 0; i != 10; ++i){
				if(m_string[pos] == digit[i] || m_string[pos] == '.'){
					number += m_string[pos];
					hasWholeNumber = false;
					break;
				}
			}

		}

		// Done. Save new position and store number
		m_position = pos;
		f = strtof(number.c_str(), 0);

		return true;
	}

	size_t StringNumbers::findNextNumber() {
		// Find next digit or sign position
		size_t posSign = m_string.size();
		size_t posDigit = m_string.size();

		// Find a sign with a consecutive digit
		for(size_t i = 0; i != 2; ++i){
			size_t p = m_string.find(sign[i], m_position);
			if(p != std::string::npos){
				bool isNextDigit = false;

				if(p + 1 < m_string.size())
					for(int i = 0; i != 10; ++i){
						if(m_string[p + 1] == digit[i]){
							isNextDigit = true;
							break;
						}
					}

				if(isNextDigit){
					posSign = std::min(p, posSign);
				}

			}

		}

		// find raw digits
		for(size_t i = 0; i != 10; ++i){
			size_t p = m_string.find(digit[i], m_position);
			if(p != std::string::npos)
				posDigit = std::min(posDigit, p);
		}

		// Get closest number number:
		return std::min(posSign, posDigit);
	}

}
/* namespace burn */
