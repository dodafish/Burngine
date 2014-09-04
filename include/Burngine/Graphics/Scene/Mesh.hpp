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
#ifndef MESH_HPP_
#define MESH_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/VertexBuffer.hpp>
#include <Burngine/Graphics/Vertex.hpp>

namespace burn {

	/**
	 * @brief Mesh holding a set of vertices with an optional Texture
	 */
	class BURNGINE_API_EXPORT Mesh {
	public:

		/**
		 * @brief Load the mesh from data
		 *
		 * @param vertices List of vertices
		 * @param size Number of vertices
		 *
		 * @return True if loading succeeded
		 */
		bool loadFromData(	const Vertex* vertices,
							const Uint32& size);

	private:
		VertexBuffer m_vertexBuffer;    ///< Buffer used by OpenGL
	};

} /* namespace burn */

#endif /* MESH_HPP_ */
