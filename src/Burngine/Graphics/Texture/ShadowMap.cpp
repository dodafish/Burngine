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

#include <Burngine/Graphics/Texture/ShadowMap.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Graphics/Scene/Model.hpp>

#define DEFAULT_RESOLUTION 512

namespace burn {

	bool ShadowMap::create(const Uint32& resolution) {

		if(!m_shadowMap.loadFromData(	Vector2ui(resolution),
										GL_RG16F,
										GL_RG,
										GL_FLOAT,
										NULL)){
			burnWarn("Cannot create shadow map texture.");
			return false;
		}

		if(!m_framebuffer.create(	Vector2ui(resolution),
									true,
									m_shadowMap)){
			burnWarn("Cannot create shadow map framebuffer.");
			return false;
		}

		// Set filtering
		m_shadowMap.setFiltering(	BaseTexture::MAG_BILINEAR,
									BaseTexture::MIN_TRILINEAR_MIPMAP);
		m_shadowMap.setSamplerParameter(GL_TEXTURE_WRAP_S,
										GL_CLAMP_TO_EDGE);
		m_shadowMap.setSamplerParameter(GL_TEXTURE_WRAP_T,
										GL_CLAMP_TO_EDGE);

		return true;
	}

	void ShadowMap::render(	const std::vector<Model*>& models,
							const Matrix4f& view,
							const Matrix4f& projection,
							bool useRawZ) {

		if(!m_shadowMap.isLoaded()){
			if(!create(DEFAULT_RESOLUTION)){
				burnErr("Could not create shadow map with default resolutions!");
			}
		}

		m_framebuffer.clear();

		if(m_framebuffer.prepare())
			for(size_t i = 0; i < models.size(); ++i)
				models[i]->renderShadowMap(	view,
											projection,
											useRawZ);

		// Blur
		m_blur.apply(	m_shadowMap,
						&m_framebuffer,
						1.f);

		// Generate mipmaps for minification filter
		m_shadowMap.generateMipmaps();

	}

	const Texture2D& ShadowMap::getTexture() const {
		return m_shadowMap;
	}

} /* namespace burn */
