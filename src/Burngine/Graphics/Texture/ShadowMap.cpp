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
#include <Burngine/Graphics/Scene/SceneNode.hpp>

#define DEFAULT_RESOLUTION 512

namespace burn {

	bool ShadowMap::create(const Uint32& resolution) {

		if(!m_shadowMap.loadFromData(	Vector2ui(resolution),
										Texture::RG16F,
										Texture::DATA_RG,
										NULL)){
			burnWarn("Cannot create shadow map texture.");
			return false;
		}

		if(!m_framebuffer.create(Vector2ui(resolution), true, m_shadowMap)){
			burnWarn("Cannot create shadow map framebuffer.");
			return false;
		}

		return true;
	}

	void ShadowMap::render(	const std::vector<SceneNode*>& sceneNodes,
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
			for(size_t i = 0; i < sceneNodes.size(); ++i)
				sceneNodes[i]->renderShadowMap(view, projection, useRawZ);

		// Blur
		m_blur.apply(m_shadowMap);

	}

	const Texture& ShadowMap::getTexture() const {
		return m_shadowMap;
	}

} /* namespace burn */
