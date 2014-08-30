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

#include <Burngine/Graphics/Gui/Transformable2D.hpp>

namespace burn {

	Transformable2D::Transformable2D() :
	m_position(0.f),
	m_rotation(0.f),
	m_scale(1.f),
	m_modelMatrix(1.f) {
		updateModelMatrix();
	}

	Transformable2D::Transformable2D(const Transformable2D& other) :
	m_position(other.m_position),
	m_rotation(other.m_rotation),
	m_scale(other.m_scale),
	m_modelMatrix(other.m_modelMatrix) {
	}

	Transformable2D& Transformable2D::operator=(const Transformable2D& other) {

		if(this == &other)
			return *this;

		m_position = other.m_position;
		m_rotation = other.m_rotation;
		m_scale = other.m_scale;
		m_modelMatrix = other.m_modelMatrix;

		return *this;
	}

	void Transformable2D::setPosition(const Vector2f& position) {
		m_position = position;
		updateModelMatrix();
	}

	const Vector2f& Transformable2D::getPosition() const {
		return m_position;
	}

	void Transformable2D::setRotation(const float& rotation) {
		m_rotation = rotation;
		updateModelMatrix();
	}

	const float& Transformable2D::getRotation() const {
		return m_rotation;
	}

	void Transformable2D::setScale(const Vector2f& scale) {
		m_scale = scale;
		updateModelMatrix();
	}

	const Vector2f& Transformable2D::getScale() const {
		return m_scale;
	}

	const Matrix4f& Transformable2D::getModelMatrix() const {
		return m_modelMatrix;
	}

	void Transformable2D::updateModelMatrix() {
		Matrix4f translationMatrix = glm::translate(m_position.x, m_position.y, 0.f);
		Matrix4f scaleMatrix = glm::scale(m_scale.x, m_scale.y, 0.f);
		Matrix4f rotationMatrix = glm::rotate(m_rotation, 0.f, 0.f, 1.f);
		m_modelMatrix = translationMatrix * rotationMatrix * scaleMatrix;
	}

} /* namespace burn */
