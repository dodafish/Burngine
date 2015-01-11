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

#ifndef BLUR_HPP_
#define BLUR_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Texture/Framebuffer.hpp>
#include <Burngine/System/NonCopyable.hpp>
#include <Burngine/Window/GlEntity.hpp>

namespace burn {

	/**
	 * @brief Blurs a texture
	 */
	class BURNGINE_API_EXPORT Blur : public GlEntity, public NonCopyable {
	public:

		Blur();

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

		/**
		 * @brief Set the blur scale. Too high values may result in bad blur.
		 */
		void setBlurScale(const float& scale);

	private:
		Framebuffer m_framebufferFirst;    ///< Framebuffer for first pass
		Texture2D m_texture;    ///< Texture for first pass
		float m_blurScale;
		Vector2ui m_resolution;    ///< Current resolution
	};

}
/* namespace burn */

#endif /* BLUR_HPP_ */
