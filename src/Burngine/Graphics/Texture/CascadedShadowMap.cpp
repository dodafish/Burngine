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

#include <Burngine/Graphics/Texture/CascadedShadowMap.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Graphics/Scene/DirectionalLight.hpp>
#include <Burngine/Graphics/Scene/SceneNode.hpp>

namespace burn {

	bool CascadedShadowMap::create(const Uint32& resolution) {

		for(int i = 0; i != 3; ++i)
			if(!m_shadowMaps[i].create(resolution)){
				burnWarn("Cannot create cascaded shadow maps.");
				return false;
			}

		return true;
	}

	void CascadedShadowMap::render(	const DirectionalLight& light,
									const std::vector<SceneNode*>& sceneNodes,
									const Vector3f& focus) {

		// Calculate light's view matrix
		Vector3f direction = light.getDirection();
		Matrix4f lightView = glm::lookAt(	focus - (direction * 500.f),
											focus - (direction * 500.f)
											- direction,
											(direction
											== Vector3f(0.f, -1.f, 0.f)) ?
											Vector3f(0.f, 0.f, -1.f) :
											Vector3f(0.f, 1.f, 0.f));

		// Render for each shadow map
		for(int i = 0; i != 3; ++i){

			// Calculate light's projection matrix
			Matrix4f lightProjection = glm::ortho(	-25.f * i * 3,
													25.f * i * 3,
													-25.f * i * 3,
													25.f * i * 3,
													-1000.f,
													0.f);

			// Render all shadow casters
			m_shadowMaps[i].render(	sceneNodes,
									lightView,
									lightProjection,
									true);

		}

	}

} /* namespace burn */
