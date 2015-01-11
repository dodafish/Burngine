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

#ifndef GLOW_HPP_
#define GLOW_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Texture/Framebuffer.hpp>
#include <Burngine/System/NonCopyable.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/Graphics/Texture/PostEffect/Blur.hpp>

namespace burn {

	class BURNGINE_API_EXPORT Glow : public GlEntity, public NonCopyable {
	public:

		Glow();

		/**
		 * @brief Apply blur effect to a texture. The final blurred image
		 * needs to be fetched with getOutput()
		 *
		 * @param texture Texture to blur
		 *
		 * @note This needs a set resolution.
		 * @see create()
		 */
		void passInput(const Texture2D& texture);

		/**
		 * @brief Gets the blurred version of the textured passed with passInput()
		 */
		void getOutput(const RenderTarget& output);

		/**
		 * @brief Setup framebuffers to run with a specific resolution.
		 *
		 * @note This (re-)creates framebuffers, so use this as little as possible.
		 */
		bool create(const Vector2ui& resolution,
					const GLint& internalFormat,
					const GLenum& dataFormat,
					const GLenum& dataType);

		/**
		 * @brief Setup blur to work properly with a texture passed as an argument.
		 * It will copy its resolution, pixel-type, data-type. Therefore, the texture
		 * has to be created.
		 *
		 * @param texture Texture to copy attributes from
		 */
		bool create(const Texture2D& texture);

	private:
		Framebuffer m_framebufferExtract;
		Texture2D m_texture;
		Blur m_blur;
		Vector2ui m_resolution;
	};

} /* namespace burn */

#endif /* GLOW_HPP_ */
