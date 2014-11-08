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

#ifndef SPRITE_HPP_
#define SPRITE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Gui/Rectangle.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <Burngine/Graphics/VertexArray.hpp>

namespace burn {

	/**
	 * @brief Rectangle displaying a texture
	 */
	class BURNGINE_API_EXPORT Sprite : public Rectangle {
	public:

		Sprite();

		/**
		 * @brief Set the texture that the sprite will draw
		 *
		 * @param texture The texture
		 * @param fitDimensions Set to true to use the texture's
		 * dimensions as the sprite's dimensions. False to keep
		 * current dimensions.
		 */
		void setTexture(const Texture& texture,
						bool fitDimensions = true);

		virtual void render(const Matrix4f& view,
							const Matrix4f& projection) const;

		virtual void render(const Shader& shader) const;

	private:

		/**
		 * @brief Update vertex buffer data
		 */
		void updateVertexData();

	private:
		Texture m_texture;
		VertexBuffer m_vertexBuffer;
		VertexArray m_vertexArray;
	};

} /* namespace burn */

#endif /* SPRITE_HPP_ */
