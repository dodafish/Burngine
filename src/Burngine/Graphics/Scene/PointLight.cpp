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

#include <Burngine/Graphics/Scene/PointLight.hpp>

namespace burn {

	PointLight::PointLight() :
	m_color(1.f),
	m_intensity(10.f) {

	}
	void PointLight::setColor(const Vector3f& color) {
		m_color = color;
	}

	const Vector3f& PointLight::getColor() const {
		return m_color;
	}

	void PointLight::setIntensity(const float& intensity) {
		m_intensity = intensity;
	}

	const float& PointLight::getIntensity() const {
		return m_intensity;
	}

} /* namespace burn */
