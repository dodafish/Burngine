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

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/System/Math.hpp>
#include <string>

namespace burn {

	/**
	 * @brief Reference to a texture loaded by TextureLoader
	 */
	class BURNGINE_API_EXPORT Texture : public GlEntity {
	public:

		/**
		 * @brief Format of a pixel
		 */
		enum PixelFormat {
			RGB = GL_RGB, RGBA = GL_RGBA, RGB16F = GL_RGB16F, RG16F = GL_RG16F
		};

		/**
		 * @brief Format of the pixeldata.
		 */
		enum DataFormat {
			DATA_RGB = GL_RGB,
			DATA_BGR = GL_BGR,
			DATA_RGBA = GL_RGBA,
			DATA_BGRA = GL_BGRA,
			DATA_RG = GL_RG
		};

	public:

		/**
		 * @brief Initialize
		 */
		Texture();

		Texture(const Texture& other);

		Texture& operator=(const Texture& other);

		~Texture();

		/**
		 * @brief Load a texture from file with TextureLoader
		 *
		 * @param file Relative path to the texture
		 *
		 * @return True if texture was loaded successfully
		 */
		bool loadFromFile(const std::string& file);

		/**
		 * @brief Load a texture from data
		 *
		 * @param dimensions width and height
		 * @param pixels How are the pixels represented? (OpenGL style)
		 * @param dataFormat How are the pixels represented in the data buffer
		 * @param dataType Sets the data type for pixels
		 * @param data The pixels or NULL for only creating the texture
		 *
		 * @note If no data is passed (data = NULL) then it will
		 * just reserve the memory for the texture and create it
		 */
		void loadFromData(	const Vector2ui& dimensions,
							const PixelFormat& pixelFormat,
							const DataFormat& dataFormat,
							const Uint8* data);

		/**
		 * @brief Get the OpenGL ID. For internal use.
		 */
		const GLuint& getId() const;

		/**
		 * @brief Is the texture loaded?
		 *
		 * @return True if the texture is loaded. False otherwise
		 */
		bool isLoaded() const;

		/**
		 * @brief Get texture's dimensions
		 */
		const Vector2ui& getDimensions() const;

	private:

		/**
		 * @brief Release OpenGL memory
		 */
		void cleanup();

	private:
		GLuint m_id;    ///< Texture ID in OpenGL
		Vector2ui m_dimensions;    ///< width and height
		Uint32* m_count;    ///< number of references to this texture
	};

} /* namespace burn */

#endif /* TEXTURE_HPP_ */
