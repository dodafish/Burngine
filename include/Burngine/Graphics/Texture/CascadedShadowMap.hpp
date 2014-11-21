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

#ifndef CASCADEDSHADOWMAP_HPP_
#define CASCADEDSHADOWMAP_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/Graphics/Texture/ShadowMap.hpp>
#include <vector>

namespace burn {

	class DirectionalLight;
	class Model;

	/**
	 * @brief Cascaded shadow mapping used for direction lights,
	 * because they cover everything.
	 */
	class BURNGINE_API_EXPORT CascadedShadowMap : public GlEntity {
	public:

		enum ShadowMapSize {
			SMALL = 0,    // Keep 0
			MEDIUM,
			WIDE    // Keep this order
		};

	public:

		/**
		 * @brief Create the shadow maps
		 *
		 * @param resolution Resolution of the shadow maps
		 *
		 * @return True on successful creation. Fales otherwise.
		 */
		bool create(const Uint32& resolution);

		/**
		 * @brief Render shadow maps for directional light
		 *
		 * @param light Directional light
		 * @param sceneNodes Possible shadow casters
		 * @param focus Point of highest detail. Typically the
		 * camera's position
		 */
		void render(const DirectionalLight& light,
					const std::vector<Model*>& models,
					const Vector3f& focus);

		/**
		 * @brief Get the shadow map of a specific area
		 *
		 * @param size Covered area of the shadowmap
		 *
		 * @return According shadow map texture
		 */
		const Texture& getShadowMap(const ShadowMapSize& size) const;

		/**
		 * @brief Get the used view matrix used for each shadow map
		 */
		const Matrix4f& getUsedViewMatrix() const;

		/**
		 * @brief Get the used projection matrix used for a specific
		 * shadow map.
		 *
		 * @param size Shadow map's size for which the projection matrix
		 * was used
		 */
		const Matrix4f& getUsedProjectionMatrix(const ShadowMapSize& size) const;

	private:
		ShadowMap m_shadowMaps[3];    ///< 3 shadow maps fetching different areas
		Matrix4f m_viewMatrix;    ///< View matrix used for each shadow map
		Matrix4f m_projectionMatrices[3];    ///< Projection matrices used for each shadow map
	};

} /* namespace burn */

#endif /* CASCADEDSHADOWMAP_HPP_ */
