//////////////////////////////////////////////////////////////////////////////
//
// Burngine is distributed under the GNU GPL v2 License
// ====================================================
//
//    Copyright (C) 2014 Dominik David (frischer-hering@gmx.de)
//
//    This program is free software; you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation; either version 2 of the License, or
//    (at your option) any later version.
//
//    This program is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License along
//    with this program; if not, write to the Free Software Foundation, Inc.,
//    51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
//
//////////////////////////////////////////////////////////////////////////////

#ifndef VIDEOMODE_HPP_
#define VIDEOMODE_HPP_

#include <Burngine/Export.hpp>

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
	VideoMode(const Vector2i& dimensions = Vector2i(400, 300));

	/**
	 * @brief Set window's dimensions
	 *
	 * @param dimensions New dimensions
	 */
	void setDimensions(const Vector2i& dimensions);

	/**
	 * @brief Get window's dimensions
	 *
	 * @return Current dimensions
	 */
	const Vector2i& getDimensions() const;

	/**
	 * @brief Set window's width
	 *
	 * @param width New width
	 */
	void setWidth(const Int32& width);

	/**
	 * @brief Get window's width
	 *
	 * @return Current width
	 */
	const Int32& getWidth() const;

	/**
	 * @brief Set window's height
	 *
	 * @param width New height
	 */
	void setHeight(const Int32& height);

	/**
	 * @brief Get window's height
	 *
	 * @return Current height
	 */
	const Int32& getHeight() const;

	/**
	 * @brief Check attributes and correct if necessary
	 *
	 * @param autoCorrect Pass true to correct invalid attributes
	 * automatically
	 *
	 * @return True if attributes are correct. False if the were not
	 * correct. Returns also false if attributes were corrected, but
	 * they are valid afterwards, as autoCorrect was enabled.
	 */
	bool checkValidity(bool autoCorrect = true);

private:
	Vector2i m_dimensions; ///< Window's dimensions
};

} /* namespace burn */

#endif /* VIDEOMODE_HPP_ */
