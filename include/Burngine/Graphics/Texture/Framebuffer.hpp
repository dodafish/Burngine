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

#ifndef FRAMEBUFFER_HPP_
#define FRAMEBUFFER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/Graphics/RenderTarget.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/System/NonCopyable.hpp>
#include <vector>

namespace burn {

	/**
	 * @brief Provides rendering into color attachments (i.e. textures)
	 * with an optional depth buffer
	 */
	class BURNGINE_API_EXPORT Framebuffer : public RenderTarget,
	public NonCopyable {
	public:

		/**
		 * @brief Initialization
		 */
		Framebuffer();

		/**
		 * @brief Cleans up
		 */
		~Framebuffer();

		/**
		 * @brief Create a framebuffer with an optional depth buffer storage
		 *
		 * @param dimensions RenderTexture's dimensions
		 * @param createDepthbuffer Set to true if you need a depth buffer, i.e. for
		 * 							depth testing
		 * @colorAttachment Framebuffers need at least one color attachment
		 * 					for creation. This attachment will be at position 0.
		 *
		 * @return Returns true on success. False otherwise
		 */
		bool create(const Vector2ui& dimensions,
					bool createDepthbuffer,
					Texture& colorAttachment);

		/**
		 * @brief Attach a texture as color attachment to the framebuffer
		 * at a specific attachment position.
		 *
		 * @param texture 	The already created texture to attach.
		 * 					If the texture was not created it will be created
		 * 					with the same dimensions as the framebuffer and with
		 * 					a 24bit RGB format.
		 * @param position The position to attach with
		 *
		 * @return Returns true if the texture was attached successfully.
		 * False otherwise.
		 */
		bool attachTexture(Texture& texture, const Uint32& position);

		/**
		 * @brief Releases OpenGL memory. Deletes RenderTexture
		 */
		void cleanup();

		/**
		 * @brief Clears the framebuffer, i.e. clears the color- and depthbuffer
		 */
		void clear();

		/**
		 * @brief Get the current dimensions of the render texture
		 *
		 * @return current dimensions
		 */
		const Vector2ui& getDimensions() const;

		/**
		 * @brief RenderTarget impl.
		 */
		virtual bool prepare() const;

		/**
		 * @brief RenderTarget impl.
		 */
		virtual Matrix4f getOrtho() const;

	private:

		/**
		 * @brief Saves the texture and its attachment position
		 */
		struct ColorAttachment {
			Texture texture;
			Uint32 position;
		};

		GLuint m_framebuffer;    ///< Container for color attachments and depthbuffer
		GLuint m_depthbuffer;    ///< Optional depth buffer
		std::vector<ColorAttachment> m_colorAttachments;    ///< Render targets
		Vector2ui m_dimensions;    ///< The framebuffers dimensions
	};

} /* namespace burn */

#endif /* FRAMEBUFFER_HPP_ */
