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

#include <Burngine/System/Rotation.hpp>
#include <glm/gtx/norm.hpp>

namespace burn {

	void Rotation::updateMatrix() {
		m_matrix = glm::toMat4(m_quaternion);
	}

	const glm::quat& Rotation::asQuaternion() const {
		return m_quaternion;
	}

	const Matrix4f& Rotation::asMatrix() const {
		return m_matrix;
	}

	void Rotation::setByEulerRadians(const Vector3f& euler) {
		m_quaternion = Quaternion(euler);
		updateMatrix();
	}

	void Rotation::setByEulerDegrees(const Vector3f& euler) {
		m_quaternion = Quaternion(glm::radians(euler));
		updateMatrix();
	}

} /* namespace burn */
