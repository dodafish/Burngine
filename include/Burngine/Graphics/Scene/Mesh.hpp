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
#include <Burngine/Graphics/VertexArray.hpp>
#include <vector>

namespace burn {

	/**
	 * @brief Mesh holding a set of vertices with an optional Texture
	 */
	class BURNGINE_API_EXPORT Mesh : public Renderable {
	public:

		Mesh();

		/**
		 * @brief Add a set of vertices
		 *
		 * @param vertices List of vertices
		 * @param size Number of vertices
		 *
		 * @return True if loading succeeded
		 *
		 * @note The data will be stored into the vertex buffer in
		 * this order: [position, normal, uv] for each vertex - the
		 * offset sizes are: [Vector3f, Vector3f, Vector2f]
		 */
		bool addData(	const Vertex* vertices,
						const Uint32& size);

		/**
		 * @brief Set material for all vertices
		 */
		void setMaterial(const Material& material);

		/**
		 * @brief Render method
		 */
		virtual void render(const Matrix4f& model,
							const Matrix4f& view,
							const Matrix4f& projection) const;

		virtual void render(const Shader& shader) const;

		/**
		 * @brief Render the object for VSM shadow map
		 *
		 * @param view Light's view
		 * @param projection Light's projection
		 */
		void renderShadowMap(	const Matrix4f& view,
								const Matrix4f& projection,
								bool useRawZ) const;

	private:

		/**
		 * @brief Ensure that the vertex array is properly set up
		 * for the current thread
		 */
		void checkVertexArray() const;

	private:

		Material m_material;    ///< Mesh has a single material

		VertexBuffer m_vertexBuffer;    ///< Buffer used by OpenGL
		Uint32 m_vertexCount;    ///< number of vertices
		Material m_material;
		VertexArray m_vertexArray;
	};

} /* namespace burn */

#endif /* MESH_HPP_ */
