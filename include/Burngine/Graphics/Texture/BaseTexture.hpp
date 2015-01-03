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
	class BURNGINE_API_EXPORT BaseTexture : public GlEntity {
	public:

		enum MagnificationFilter {
			MAG_NEAREST, MAG_BILINEAR
		};

		enum MinificationFilter {
			MIN_NEAREST, MIN_BILINEAR, MIN_NEAREST_MIPMAP, MIN_BILINEAR_MIPMAP, MIN_TRILINEAR_MIPMAP
		};

	public:

		/**
		 * @brief Initialize general properties
		 */
		BaseTexture();

		/**
		 * @brief Copy general properties and increase reference count
		 */
		BaseTexture(const BaseTexture& other);

		/**
		 * @brief Copy general properties and increase reference count
		 */
		BaseTexture& operator=(const BaseTexture& other);

		/**
		 * @brief Destroy reference. Cleanup memory if it is the last one.
		 */
		virtual ~BaseTexture();

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

		/**
		 * @brief Bind the texture to a specific unit
		 */
		virtual void bind(const Uint32& unit) const = 0;

		const GLint& getInternalFormat() const;
		const GLenum& getDataFormat() const;
		const GLenum& getDataType() const;

		void setFiltering(	const MagnificationFilter& mag,
							const MinificationFilter& min);

		const MagnificationFilter& getMagnificationFilter() const;
		const MinificationFilter& getMinificationFilter() const;

		void setSamplerParameter(	const GLenum& parameter,
									const GLenum& value);

	protected:

		/**
		 * @brief Release OpenGL memory
		 */
		void cleanup();

	protected:
		GLuint m_id;    ///< Texture ID in OpenGL
		Vector2ui m_dimensions;    ///< width and height
		GLint m_internalFormat;	///< internal format for opengl
		GLenum m_dataFormat;	///< format of the pixels being loaded
		GLenum m_dataType;		///< Type of data (byte, float, etc.)
		Uint32* m_count;    ///< number of references to this texture
		GLuint m_samplerId;    ///< ID for texture sampler
		MagnificationFilter m_magFilter;
		MinificationFilter m_minFilter;
	};

} /* namespace burn */

#endif /* TEXTURE_HPP_ */
