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

#include <Burngine/Graphics/Transformable.hpp>

namespace burn {

	Transformable::Transformable() :
	m_position(0.f),
	m_rotation(0.f),
	m_scale(1.f),
	m_modelMatrix(1.f) {
		updateModelMatrix();
	}

	Transformable::Transformable(const Transformable& other) :
	m_position(other.m_position),
	m_rotation(other.m_rotation),
	m_scale(other.m_scale),
	m_modelMatrix(other.m_modelMatrix) {
	}

	Transformable& Transformable::operator=(const Transformable& other) {

		if(this == &other)
			return *this;

		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scale = other.m_scale;
		m_modelMatrix = other.m_modelMatrix;

		return *this;
	}

	void Transformable::setPosition(const Vector3f& position) {
		m_position = position;
		updateModelMatrix();
	}

	const Vector3f& Transformable::getPosition() const {
		return m_position;
	}

	void Transformable::setRotation(const Vector3f& rotation) {
		m_rotation = rotation;
		updateModelMatrix();
	}

	const Vector3f& Transformable::getRotation() const {
		return m_rotation;
	}

	void Transformable::setScale(const Vector3f& scale) {
		m_scale = scale;
		updateModelMatrix();
	}

	const Vector3f& Transformable::getScale() const {
		return m_scale;
	}

	const Matrix4f& Transformable::getModelMatrix() const {
		return m_modelMatrix;
	}

	void Transformable::updateModelMatrix() {
		Matrix4f translationMatrix = glm::translate(m_position.x, m_position.y, m_position.z);
		Matrix4f scaleMatrix = glm::scale(m_scale.x, m_scale.y, m_scale.z);
		Matrix4f rotationMatrix = glm::rotate(m_rotation.x, 1.f, 0.f, 0.f);
		rotationMatrix *= glm::rotate(m_rotation.y, 0.f, 1.f, 0.f);
		rotationMatrix *= glm::rotate(m_rotation.z, 0.f, 0.f, 1.f);
		m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}

} /* namespace burn */
