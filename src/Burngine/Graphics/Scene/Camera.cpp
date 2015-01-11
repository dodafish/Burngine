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
#include <Burngine/System/RotationUtil.hpp>

namespace burn {

	Camera::Camera(	const float& aspectRatio,
					const float& fov) :
	m_aspectRatio(aspectRatio),
	m_fieldOfView(fov) {
	}

	void Camera::setAspectRatio(const float& ratio) {
		m_aspectRatio = ratio;
	}

	const float& Camera::getAspectRatio() const {
		return m_aspectRatio;
	}

	void Camera::lookAt(const Vector3f& focus) {
		setRotation(Rotation(RotationUtil::RotationBetweenVectors(	Vector3f(0.f, 0.f, -1.f),
																	focus - getPosition())));
	}

	void Camera::setFov(const float& fov) {
		m_fieldOfView = fov;
	}

	const float& Camera::getFov() const {
		return m_fieldOfView;
	}

	Matrix4f Camera::getViewMatrix() const {
		return glm::lookAt(	getPosition(),
							getPosition()
							+ Vector3f(getRotation().asMatrix() * Vector4f(0.f, 0.f, -1.f, 1.f)),
							Vector3f(0.f, 1.f, 0.f));
	}

	Matrix4f Camera::getProjectionMatrix() const {
		return glm::perspective<float>(getFov(), getAspectRatio(), 0.01f, 10000.f);
	}

} /* namespace burn */
