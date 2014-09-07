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

#ifndef RENDERTEXTURE_HPP_
#define RENDERTEXTURE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/Graphics/RenderTarget.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/System/NonCopyable.hpp>

namespace burn {

	/**
	 * @brief Provides rendering into a texture
	 */
	class BURNGINE_API_EXPORT RenderTexture : public RenderTarget,
	public NonCopyable {
	public:

		/**
		 * @brief Initialization
		 */
		RenderTexture();

		/**
		 * @brief Cleans up
		 */
		~RenderTexture();

		/**
		 * @brief Create the rendertexture
		 *
		 * @param dimensions RenderTexture's dimensions
		 *
		 * @return Returns true on success. False otherwise
		 */
		bool create(const Vector2ui& dimensions);

		/**
		 * @brief Releases OpenGL memory. Deletes RenderTexture
		 */
		void cleanup();

		/**
		 * @brief Clears the rendertexture, i.e. clears the buffers
		 */
		void clear();

		/**
		 * @brief Get the texture into which was rendered
		 *
		 * @return Rendered texture
		 */
		const Texture& getTexture() const;

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
		GLuint m_framebuffer;    // Container for texture and depthbuffer
		GLuint m_depthbuffer;
		Texture m_texture;
		Vector2ui m_dimensions;
	};

} /* namespace burn */

#endif /* RENDERTEXTURE_HPP_ */
