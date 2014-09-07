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

#include <Burngine/Graphics/Scene/Transformable3D.hpp>

namespace burn {

	Transformable3D::Transformable3D() :
	m_position(0.f),
	m_rotation(0.f),
	m_scale(1.f),
	m_modelMatrix(1.f),
	m_parent(NULL) {
		updateModelMatrix();
	}

	Transformable3D::Transformable3D(const Transformable3D& other) :
	m_position(other.m_position),
	m_rotation(other.m_rotation),
	m_scale(other.m_scale),
	m_modelMatrix(other.m_modelMatrix),
	m_parent(other.m_parent) {
	}

	Transformable3D& Transformable3D::operator=(const Transformable3D& other) {

		if(this == &other)
			return *this;

		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scale = other.m_scale;
		m_modelMatrix = other.m_modelMatrix;
		m_parent = other.m_parent;

		return *this;
	}

	void Transformable3D::setParent(Transformable3D& parent) const {
		m_parent = &parent;
	}

	void Transformable3D::setPosition(const Vector3f& position) {
		m_position = position;
		updateModelMatrix();
	}

	const Vector3f& Transformable3D::getPosition() const {
		return m_position;
	}

	void Transformable3D::setRotation(const Vector3f& rotation) {
		m_rotation = rotation;
		updateModelMatrix();
	}

	const Vector3f& Transformable3D::getRotation() const {
		return m_rotation;
	}

	void Transformable3D::setScale(const Vector3f& scale) {
		m_scale = scale;
		updateModelMatrix();
	}

	const Vector3f& Transformable3D::getScale() const {
		return m_scale;
	}

	Matrix4f Transformable3D::getModelMatrix() const {
		if(m_parent != NULL){
			return m_modelMatrix * m_parent->getModelMatrix();
		}
		return m_modelMatrix;
	}

	void Transformable3D::updateModelMatrix() {
		Matrix4f translationMatrix = glm::translate(m_position.x,
													m_position.y,
													m_position.z);
		Matrix4f scaleMatrix = glm::scale(m_scale.x, m_scale.y, m_scale.z);
		Matrix4f rotationMatrix = glm::rotate(m_rotation.x, 1.f, 0.f, 0.f);
		rotationMatrix *= glm::rotate(m_rotation.y, 0.f, 1.f, 0.f);
		rotationMatrix *= glm::rotate(m_rotation.z, 0.f, 0.f, 1.f);
		m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}

} /* namespace burn */
