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

#include <Burngine/Window/VideoMode.hpp>

namespace burn {

	VideoMode::VideoMode(const Vector2ui& dimensions) :
	m_dimensions(dimensions) {
		checkValidity();
	}

	void VideoMode::setDimensions(const Vector2ui& dimensions) {
		m_dimensions = dimensions;
		checkValidity();
	}

	const Vector2ui& VideoMode::getDimensions() const {
		return m_dimensions;
	}

	void VideoMode::setWidth(const Uint32& width) {
		m_dimensions.x = width;
		checkValidity();
	}

	const Uint32& VideoMode::getWidth() const {
		return m_dimensions.x;
	}

	void VideoMode::setHeight(const Uint32& height) {
		m_dimensions.y = height;
		checkValidity();
	}

	const Uint32& VideoMode::getHeight() const {
		return m_dimensions.y;
	}

	void VideoMode::checkValidity() {

		// Check dimensions
		// Minimum width/height is assumed to be 1 to avoid
		// complications with width/height = 0
		if(m_dimensions.x < 1){
			m_dimensions.x = 1;
		}
		if(m_dimensions.y < 1){
			m_dimensions.y = 1;
		}

	}

} /* namespace burn */
