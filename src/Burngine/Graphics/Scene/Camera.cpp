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

#include <Burngine/Graphics/Scene/Camera.hpp>

namespace burn {

	void Camera::setAspectRatio(const float& ratio) {
		m_aspectRatio = ratio;
	}

	const float& Camera::getAspectRatio() const {
		return m_aspectRatio;
	}

	void Camera::setFocus(const Vector3f& focus) {
		if(focus == getPosition())
			m_focus += Vector3f(0.f, 0.f, -0.1f);
		m_focus = focus;
	}

	const Vector3f& Camera::getFocus() const {
		return m_focus;
	}

	void Camera::setFov(const float& fov) {
		m_fieldOfView = fov;
	}

	const float& Camera::getFov() const {
		return m_fieldOfView;
	}

} /* namespace burn */
