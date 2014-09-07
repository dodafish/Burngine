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
#include <Burngine/Graphics/Scene/Material.hpp>
#include <Burngine/Graphics/Scene/SceneNode.hpp>
#include <Burngine/Graphics/VertexArray.hpp>
#include <vector>

namespace burn {

	/**
	 * @brief Mesh holding a set of vertices with an optional Texture
	 */
	class BURNGINE_API_EXPORT Mesh : public SceneNode {
	public:

		Mesh();

		/**
		 * @brief Add a set of vertices with a material
		 *
		 * @param vertices List of vertices
		 * @param size Number of vertices
		 * @param material Material used for rendering these vertices
		 *
		 * @return True if loading succeeded
		 *
		 * @note The data will be stored into the vertex buffer in
		 * this order: [position, normal, uv] for each vertex - the
		 * offset sizes are: [Vector3f, Vector3f, Vector2f]
		 */
		bool addData(	const Vertex* vertices,
							const Uint32& size,
							const Material& material);

		/**
		 * @brief Set material for all vertices
		 */
		void setMaterial(const Material& material);

		/**
		 * @brief Render method
		 */
		virtual void render(const Matrix4f& view,
							const Matrix4f& projection) const;

	private:

		/**
		 * @brief Assigns vertices to a material by range
		 */
		struct VertexMaterial {
			size_t first;
			size_t count;
			Material material;
		};
		std::vector<VertexMaterial> m_vertexMaterials;

		VertexBuffer m_vertexBuffer;    ///< Buffer used by OpenGL
		Uint32 m_vertexCount;    ///< number of vertices
		Material m_material;
		VertexArray m_vertexArray;
	};

} /* namespace burn */

#endif /* MESH_HPP_ */
