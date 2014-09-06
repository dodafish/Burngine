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

#ifndef STRINGNUMBERS_HPP_
#define STRINGNUMBERS_HPP_

#include <Burngine/Export.hpp>
#include <string>

namespace burn {

	/**
	 * @brief Extracts numbers from a string
	 */
	class BURNGINE_API_EXPORT StringNumbers {
	public:

		/**
		 * @brief Simple initialization
		 */
		StringNumbers();

		/**
		 * @brief Set a new string with numbers to extract from
		 *
		 * @param s String with numbers
		 */
		void setString(const std::string& s);

		/**
		 * @brief Extract the next number being an integer
		 *
		 * @param n Integer to store the extracted number
		 *
		 * @return True if an integer was extracted successfully
		 *
		 * @note If the next occurency is a floating number then it
		 * will be converted to an integer value. Thus, there is
		 * a loss of precision.
		 */
		bool nextInt(int& n);

		/**
		 * @brief Extract the next number being a float
		 *
		 * @param n Float to store the extracted number
		 *
		 * @return True if an float was extracted successfully
		 */
		bool nextFloat(float& f);

	private:

		/// @brief Index of first digit or a sign
		size_t findNextNumber();

	private:
		std::string m_string;    ///< String with numbers
		size_t m_position;    ///< Position behind last extracted number
	};

} /* namespace burn */

#endif /* STRINGNUMBERS_HPP_ */
