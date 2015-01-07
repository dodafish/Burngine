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

#ifndef INCLUDE_BURNGINE_GRAPHICS_SCENE_BILLBOARD_HPP_
#define INCLUDE_BURNGINE_GRAPHICS_SCENE_BILLBOARD_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Transformable3D.hpp>
#include <Burngine/Graphics/Texture/Texture2D.hpp>
#include <Burngine/Graphics/VertexBuffer.hpp>
#include <Burngine/Graphics/VertexArray.hpp>
#include <Burngine/Window/GlEntity.hpp>

namespace burn {

	/**
	 * @brief 2D Sprite placed in the scene oriented towards the camera (by Renderer)
	 */
	class BURNGINE_API_EXPORT Billboard : public Transformable3D, public GlEntity {
	public:

		Billboard();

		void render(const Matrix4f& view,
					const Matrix4f& projection) const;

		void setTexture(const Texture2D& texture);

		const Texture2D& getTexture() const;

	protected:

		/**
		 * @brief Update vertex buffer data
		 */
		void updateVertexData();

		/**
		 * @brief Update vertex array object if necessary
		 */
		void ensureUpdatedVertexArray() const;

	private:
		Texture2D m_texture;
		Vector2f m_dimensions;
		Vector4f m_color;    ///< Rectangle's color
		VertexBuffer m_vertexBuffer;    ///< Vertex buffer
		VertexArray m_vertexArray;    ///< Vertex array
	};

} /* namespace burn */

#endif /* INCLUDE_BURNGINE_GRAPHICS_SCENE_BILLBOARD_HPP_ */
