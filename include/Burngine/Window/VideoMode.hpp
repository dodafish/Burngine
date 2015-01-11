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

#ifndef VIDEOMODE_HPP_
#define VIDEOMODE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/Math.hpp>

namespace burn {

	/**
	 * @brief Defines window's video mode.
	 * Has to be passed at window creation or via
	 * Window::setVideoMode()
	 */
	class BURNGINE_API_EXPORT VideoMode {
	public:

		/**
		 * @brief Default constructor. Optionally, the dimensions can
		 * already be set.
		 *
		 * @param dimensions Window's dimensions. Default: 400x300
		 */
		VideoMode(const Vector2ui& dimensions = Vector2ui(400, 300));

		/**
		 * @brief Set window's dimensions
		 *
		 * @param dimensions New dimensions
		 */
		void setDimensions(const Vector2ui& dimensions);

		/**
		 * @brief Get window's dimensions
		 *
		 * @return Current dimensions
		 */
		const Vector2ui& getDimensions() const;

		/**
		 * @brief Set window's width
		 *
		 * @param width New width
		 */
		void setWidth(const Uint32& width);

		/**
		 * @brief Get window's width
		 *
		 * @return Current width
		 */
		const Uint32& getWidth() const;

		/**
		 * @brief Set window's height
		 *
		 * @param width New height
		 */
		void setHeight(const Uint32& height);

		/**
		 * @brief Get window's height
		 *
		 * @return Current height
		 */
		const Uint32& getHeight() const;

	private:

		/**
		 * @brief Check attributes and correct if necessary
		 */
		void checkValidity();

	private:
		Vector2ui m_dimensions;    ///< Window's dimensions
	};

} /* namespace burn */

#endif /* VIDEOMODE_HPP_ */
