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

#ifndef ERROR_HPP_
#define ERROR_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/NonInstancable.hpp>
#include <string>

namespace burn {

	/**
	 * @brief Error logging. Optimized for platform, i.e. using message boxes.
	 * Use only for fatal errors, because an invocation will terminate the execution
	 */
	class BURNGINE_API_EXPORT Error : public NonInstancable {
	public:

		/**
		 * @brief Log an error string
		 *
		 * @param msg Error message
		 */
		static void log(const std::string& msg,
						const char* file,
						int line);

	};

/////////////////////////////////////////////////////////////////////////////
// Shortcut:
#define burnErr(msg) Error::log(msg, __FILE__, __LINE__)

} /* namespace burn */

#endif /* ERROR_HPP_ */
