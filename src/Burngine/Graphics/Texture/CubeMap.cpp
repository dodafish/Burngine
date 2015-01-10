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

#include <Burngine/Graphics/Texture/CubeMap.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Graphics/Texture/SOIL.h>
#include <sstream>

namespace burn {

	bool CubeMap::loadFromFile(	const std::string& fileXPos,
								const std::string& fileXNeg,
								const std::string& fileYPos,
								const std::string& fileYNeg,
								const std::string& fileZPos,
								const std::string& fileZNeg) {

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
		m_id = SOIL_load_OGL_cubemap(	fileXPos.c_str(),
										fileXNeg.c_str(),
										fileYPos.c_str(),
										fileYNeg.c_str(),
										fileZPos.c_str(),
										fileZNeg.c_str(),
										SOIL_LOAD_AUTO,
										SOIL_CREATE_NEW_ID,
										SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB
										| SOIL_FLAG_COMPRESS_TO_DXT);

		/* check for an error during the load process */
		if(0 == m_id){
			std::stringstream ss;
			ss << "Failed loading cube map '" << fileXPos << "' (and others): " << SOIL_last_result();
			burnWarn(ss.str());
			return false;
		}

		// Set data information
		m_internalFormat = GL_RGBA;
		m_dataFormat = GL_RGBA;
		m_dataType = GL_UNSIGNED_BYTE;

		// Fetch dimensions
		glBindTexture( 	GL_TEXTURE_CUBE_MAP,
						m_id);

		int w, h;
		glGetTexLevelParameteriv( 	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
									0,
									GL_TEXTURE_WIDTH,
									&w);
		glGetTexLevelParameteriv( 	GL_TEXTURE_CUBE_MAP_POSITIVE_X,
									0,
									GL_TEXTURE_HEIGHT,
									&h);

		m_dimensions = Vector2ui(	w,
									h);

		// Create sampler
		glGenSamplers(	1,
						&m_samplerId);

		// Set sampling parameters
		setFiltering(	BaseTexture::MAG_BILINEAR,
						BaseTexture::MIN_TRILINEAR_MIPMAP);

		// Unbind cube map
		glBindTexture( 	GL_TEXTURE_CUBE_MAP,
						0);

		return true;
	}

	void CubeMap::bind(const Uint32& unit) const {

		if(!isLoaded())
			return;

		ensureContext();
		glActiveTexture(GL_TEXTURE0 + unit);
		glBindTexture( 	GL_TEXTURE_CUBE_MAP,
						m_id);
		glBindSampler(	unit,
						m_samplerId);

	}

} /* namespace burn */
