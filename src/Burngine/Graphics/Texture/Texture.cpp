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

#include <Burngine/Graphics/Texture/Texture.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Graphics/Texture/SOIL.h>
#include <sstream>

namespace burn {

	Texture::Texture() :
	m_id(0),
	m_dimensions(0),
	m_count(new Uint32(1)) {

	}

	Texture::Texture(const Texture& other) :
	GlEntity(other),
	m_id(other.m_id),
	m_dimensions(other.m_dimensions),
	m_count(other.m_count) {

		++(*m_count);

	}

	Texture& Texture::operator=(const Texture& other) {

		if(this == &other)
			return *this;

		--(*m_count);
		if((*m_count) == 0){
			cleanup();
			delete m_count;
		}

		m_id = other.m_id;
		m_dimensions = other.m_dimensions;
		m_count = other.m_count;

		++(*m_count);

		return *this;
	}

	Texture::~Texture() {

		--(*m_count);
		if((*m_count) == 0){
			cleanup();
			delete m_count;
		}

	}

	void Texture::cleanup() {

		if(m_id == 0){
			ensureContext();
			glDeleteTextures(1, &m_id);

			m_id = 0;
		}

	}

	bool Texture::loadFromFile(const std::string& file) {

		// Handle this as a whole new texture
		if((*m_count) == 1){
			cleanup();
		}else{
			--(*m_count);
			m_count = new Uint32(1);
			m_id = 0;
		}

		ensureContext();
		m_id = SOIL_load_OGL_texture(	file.c_str(),
										SOIL_LOAD_AUTO,
										SOIL_CREATE_NEW_ID,
										SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y
										| SOIL_FLAG_NTSC_SAFE_RGB
										| SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if(0 == m_id){
			std::stringstream ss;
			ss << "Failed loading texture '" << file << "': "
			<< SOIL_last_result();
			burnWarn(ss.str());
			return false;
		}

		// Fetch dimensions
		glBindTexture(GL_TEXTURE_2D, m_id);

		int w, h;
		glGetTexLevelParameteriv( GL_TEXTURE_2D, 0,
		GL_TEXTURE_WIDTH,
									&w);
		glGetTexLevelParameteriv( GL_TEXTURE_2D, 0,
		GL_TEXTURE_HEIGHT,
									&h);

		m_dimensions = Vector2ui(w, h);

		// Set sampler parameters
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glBindTexture(GL_TEXTURE_2D, 0);

		return true;
	}

	bool Texture::loadFromData(	const Vector2ui& dimensions,
								const PixelFormat& pixelFormat,
								const DataFormat& dataFormat,
								const Uint8* data) {

		ensureContext();

		// Check parameters
		if(dimensions.x == 0 || dimensions.y == 0){
			burnWarn("Invalid dimensions. Cannot create texture.");
			return false;
		}
		if(dimensions.x > GL_MAX_TEXTURE_SIZE || dimensions.y > GL_MAX_TEXTURE_SIZE){
			burnWarn("Texture dimensions are too big.");
			return false;
		}


		// Handle this as a whole new texture
		if((*m_count) == 1){
			cleanup();
		}else{
			--(*m_count);
			m_count = new Uint32(1);
			m_id = 0;
		}

		// Copy information
		m_dimensions = dimensions;

		// Create new texture

		glGenTextures(1, &m_id);
		glBindTexture( GL_TEXTURE_2D, m_id);

		unsigned int dataType = GL_UNSIGNED_BYTE;
		if(pixelFormat == RGB16F || pixelFormat == RG16F)
			dataType = GL_FLOAT;

		glTexImage2D( 	GL_TEXTURE_2D,
						0,
						pixelFormat,
						m_dimensions.x,
						m_dimensions.y,
						0,
						dataFormat,
						dataType,
						data);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glBindTexture( GL_TEXTURE_2D, 0);

		return true;
	}

	const GLuint& Texture::getId() const {
		return m_id;
	}

	bool Texture::isLoaded() const {
		return m_id != 0;
	}

	const Vector2ui& Texture::getDimensions() const {
		return m_dimensions;
	}

} /* namespace burn */
