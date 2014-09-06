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

#include <Burngine/Graphics/Scene/Mesh.hpp>
#include <Burngine/System/Error.hpp>

namespace burn {

	bool Mesh::loadFromData(const Vertex* vertices,
							const Uint32& size) {

		// Check input:
		// Pointer has to be valid
		// Mesh has to be at least one triangle = 3 vertices
		if(vertices == NULL || size < 3){
			burnWarn("Cannot load mesh! Vertices are invalid or too small.");
			return false;
		}

		// Reset the vertex buffer
		m_vertexBuffer.reset();

		// Add the vertices
		for(Uint32 i = 0; i < size; ++i){
			m_vertexBuffer.addData(	&((vertices + i)->getPosition()),
									sizeof(Vector3f));
			m_vertexBuffer.addData(	&((vertices + i)->getNormal()),
									sizeof(Vector3f));
			m_vertexBuffer.addData(	&((vertices + i)->getUv()),
									sizeof(Vector2f));
		}

		m_vertexCount = size;

		return true;
	}

	const VertexBuffer& Mesh::getVertexBuffer() const {
		return m_vertexBuffer;
	}

	const Uint32& Mesh::getVertexCount() const {
		return m_vertexCount;
	}

} /* namespace burn */
