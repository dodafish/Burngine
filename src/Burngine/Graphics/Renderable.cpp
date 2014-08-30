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

#include <Burngine/Graphics/Renderable.hpp>

namespace burn {

	Renderable::Renderable() :
	m_vao(0) {
		ensureContext();
		glGenVertexArrays(1, &m_vao);
	}

	Renderable::Renderable(const Renderable& other) :
	GlEntity(other),
	Transformable(other),
	m_vao(0) {
		ensureContext();
		glGenVertexArrays(1, &m_vao);
	}

	Renderable::~Renderable() {
		ensureContext();
		glDeleteVertexArrays(1, &m_vao);
	}

	void Renderable::bindVao() const {
		ensureContext();
		glBindVertexArray(m_vao);
	}

	void Renderable::unbindVao() const {
		ensureContext();
		glBindVertexArray(0);
	}

} /* namespace burn */
