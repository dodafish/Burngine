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
#include <Burngine/Graphics/Scene/Model.hpp>

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
									const std::vector<Model*>& models,
									const Vector3f& focus) {

		ensureContext();
		glDisable(GL_CULL_FACE);

		// Calculate light's view matrix
		Vector3f direction = glm::normalize(light.getDirection());
		Matrix4f lightView = glm::lookAt(focus, focus - direction, (direction == Vector3f(0.f, -1.f, 0.f)) ?
		Vector3f(0.f, 0.f, -1.f) : Vector3f(0.f, 1.f, 0.f));

		// Store used matrix
		m_viewMatrix = lightView;

		float areas[3] = {10.f, 35.f, 100.f};
		// Render for each shadow map
		for(int i = 0; i != 3; ++i){

			// Calculate light's projection matrix
			Matrix4f lightProjection = glm::ortho<float>(-areas[i], areas[i], -areas[i], areas[i], -500.f, 500.f);

			// Render all shadow casters
			m_shadowMaps[i].render(models, lightView, lightProjection, true);

			// Store used matrix
			m_projectionMatrices[i] = lightProjection;

		}

	}

	const Texture2D& CascadedShadowMap::getShadowMap(const ShadowMapSize& size) const {
		return m_shadowMaps[size].getTexture();
	}

	const Matrix4f& CascadedShadowMap::getUsedViewMatrix() const {
		return m_viewMatrix;
	}

	const Matrix4f& CascadedShadowMap::getUsedProjectionMatrix(const ShadowMapSize& size) const {
		return m_projectionMatrices[size];
	}

} /* namespace burn */
