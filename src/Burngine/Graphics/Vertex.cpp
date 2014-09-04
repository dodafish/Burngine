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

#include <Burngine/Graphics/Vertex.hpp>

namespace burn {

	Vertex::Vertex(	const Vector3f& position,
					const Vector3f& normal,
					const Vector2f& uv) :
	m_position(position),
	m_normal(normal),
	m_uv(uv) {

	}

	void Vertex::setPosition(const Vector3f& position) {
		m_position = position;
	}

	const Vector3f& Vertex::getPosition() const {
		return m_position;
	}

	void Vertex::setNormal(const Vector3f& normal) {
		m_normal = normal;
	}

	const Vector3f& Vertex::getNormal() const {
		return m_normal;
	}

	void Vertex::setUv(const Vector2f& uv) {
		m_uv = uv;
	}

	const Vector2f& Vertex::getUv() const {
		return m_uv;
	}

} /* namespace burn */
