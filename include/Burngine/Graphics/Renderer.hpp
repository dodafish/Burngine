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

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/Graphics/Texture/Framebuffer.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <Burngine/Graphics/VertexBuffer.hpp>
#include <Burngine/Graphics/VertexArray.hpp>
#include <Burngine/Graphics/Scene/Scene.hpp>

namespace burn {

	class RenderTarget;
	class GuiNode;
	class SceneNode;
	class Camera;
	class PointLight;
	class DirectionalLight;
	class SpotLight;
	class Shader;

	/**
	 * @brief Renders Renderables into a RenderTarget
	 */
	class BURNGINE_API_EXPORT Renderer : public GlEntity {
	public:

		enum Output {
			FINAL,    ///< Output final result
			DIFFUSE,    ///< Output raw colors
			NORMAL,    ///< Output normals
			POSITION,    ///< Output positions
			LIGHTING    ///< Output lighting
		};

	public:

		/**
		 * @brief Creates render textures
		 */
		Renderer();

		/**
		 * @brief Prepare for rendering. Adjust render textures
		 *
		 * @param targetDimensions dimensions of the final render target
		 */
		void prepare(const Vector2ui& targetDimensions);

		/**
		 * @brief Put the render result into target
		 *
		 * @param target final render target
		 * @param output The output that is used
		 */
		void finalize(	const RenderTarget& target,
						const Output& output = FINAL);

		/**
		 * @brief Render a GuiNode
		 */
		void renderGuiNode(const GuiNode& node);

		/**
		 * @brief Render a Scene
		 */
		void renderScene(	const Scene& scene,
							const Camera& camera);

	private:

		/**
		 * @brief Render a SceneNode
		 */
		void renderSceneNode(	const SceneNode& node,
								const Camera& camera);

		/**
		 * @brief Render lighting based on current g-buffer data
		 */
		void renderPointLight(const PointLight& pointLight);

		/**
		 * @brief Render lighting based on current g-buffer data
		 */
		void renderDirectionalLight(const DirectionalLight& directionalLight);

		/**
		 * @brief Render lighting based on current g-buffer data
		 */
		void renderSpotLight(const SpotLight& spotLight);

		/**
		 * @brief Render a fullscreen quad with texture coords
		 */
		void renderLighting(const Shader& shader);

	private:
		// G-Buffer:
		Framebuffer m_gBuffer;    ///< Framebuffer storing geometrical information
		Texture m_diffuseTexture;
		Texture m_normalTexture;
		Texture m_positionTexture;
		// Light rendering:
		Framebuffer m_lightingBuffer;    ///< Framebuffer used for light rendering
		Texture m_diffuseLighting;
		Texture m_shadowMap;    ///< VSM for directional and spot lights
		// Helper:
		VertexBuffer m_fullscreenQuadBuffer;
		VertexArray m_fullscreenQuadVertexArray;
	};

} /* namespace burn */

#endif /* RENDERER_HPP_ */
