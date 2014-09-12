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

#include <Burngine/Graphics/Scene/DirectionalLight.hpp>

namespace burn {

	Vector3f DirectionalLight::getDirection() const {
		Matrix4f rotationMatrix = glm::rotate(getRotation().x, 1.f, 0.f, 0.f);
		rotationMatrix *= glm::rotate(getRotation().y, 0.f, 1.f, 0.f);
		rotationMatrix *= glm::rotate(getRotation().z, 0.f, 0.f, 1.f);
		return Vector3f(Vector4f(0.f, -1.f, 0.f, 1.f) * rotationMatrix)
		* (-1.f);
	}

} /* namespace burn */
