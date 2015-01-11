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

#include <Burngine/Graphics/Texture/Texture2D.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Graphics/Texture/SOIL.h>
#include <sstream>
#include <iostream>

namespace {
	burn::Vector2ui MAX_TEXTURE_SIZE;
}

namespace burn {

	void Texture2D::onOpenGlInit() {

		// Determine the maximum texture size
		ensureContext();
		GLint maxTextureSize = 0;
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
		MAX_TEXTURE_SIZE = Vector2ui(maxTextureSize);
		std::cout << "Max texture2D size: " <<
		MAX_TEXTURE_SIZE.x << "x" << MAX_TEXTURE_SIZE.y << "\n";

	}

	const Vector2ui& Texture2D::getMaxTextureSize() {
		return MAX_TEXTURE_SIZE;
	}

	bool Texture2D::loadFromFile(const std::string& file) {

		// Handle this as a whole new texture
		if((*m_count) == 1){
			cleanup();
		}else{
			--(*m_count);
			m_count = new Uint32(1);
			m_id = 0;
			m_samplerId = 0;
		}

		ensureContext();
		m_id = SOIL_load_OGL_texture(file.c_str(),
										SOIL_LOAD_AUTO,
										SOIL_CREATE_NEW_ID,
										SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
										| SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if(0 == m_id){
			std::stringstream ss;
			ss << "Failed loading texture '" << file << "': " << SOIL_last_result();
			burnWarn(ss.str());
			return false;
		}

		// Set data information
		m_internalFormat = GL_RGBA8;
		m_dataFormat = GL_RGBA;
		m_dataType = GL_UNSIGNED_BYTE;

		// Fetch dimensions
		glBindTexture( GL_TEXTURE_2D,
						m_id);

		int w, h;
		glGetTexLevelParameteriv( GL_TEXTURE_2D,
									0,
									GL_TEXTURE_WIDTH,
									&w);
		glGetTexLevelParameteriv( GL_TEXTURE_2D,
									0,
									GL_TEXTURE_HEIGHT,
									&h);

		m_dimensions = Vector2ui(w,
									h);

		// Create sampler
		glGenSamplers(1,
						&m_samplerId);

		// Set sampling parameters
		setFiltering(BaseTexture::MAG_BILINEAR,
						BaseTexture::MIN_TRILINEAR_MIPMAP);

		glBindTexture( GL_TEXTURE_2D,
						0);

		return true;
	}

	bool Texture2D::loadFromData(	const Vector2ui& dimensions,
									const GLint& internalFormat,
									const GLenum& dataFormat,
									const GLenum& dataType,
									const Uint8* data) {

		ensureContext();

		// Check parameters
		if(dimensions.x == 0 || dimensions.y == 0){
			burnWarn("Invalid dimensions. Cannot create texture.");
			return false;
		}
		if(dimensions.x > MAX_TEXTURE_SIZE.x || dimensions.y > MAX_TEXTURE_SIZE.y){
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
			m_samplerId = 0;
		}

		// Copy information
		m_dimensions = dimensions;
		m_internalFormat = internalFormat;
		m_dataFormat = dataFormat;
		m_dataType = dataType;

		// Create new texture
		glGenTextures(1,
						&m_id);
		glBindTexture( GL_TEXTURE_2D,
						m_id);

		glTexImage2D( GL_TEXTURE_2D,
						0,
						internalFormat,
						m_dimensions.x,
						m_dimensions.y,
						0,
						dataFormat,
						dataType,
						data);

		// Create sampler
		glGenSamplers(1,
						&m_samplerId);

		// Set sampling parameters
		setFiltering(BaseTexture::MAG_BILINEAR,
						BaseTexture::MIN_BILINEAR);

		glBindTexture( GL_TEXTURE_2D,
						0);

		return true;
	}

	void Texture2D::bind(const Uint32& unit) const {

		if(!isLoaded())
			return;

		ensureContext();
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture( GL_TEXTURE_2D,
						m_id);
		glBindSampler(unit,
						m_samplerId);

	}

} /* namespace burn */
