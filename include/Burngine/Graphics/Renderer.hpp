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
#include <Burngine/Graphics/VertexBuffer.hpp>
#include <Burngine/Graphics/VertexArray.hpp>
#include <Burngine/Graphics/Scene/Scene.hpp>
#include <Burngine/Graphics/Texture/Texture2D.hpp>
#include <Burngine/Graphics/Texture/CascadedShadowMap.hpp>
#include <Burngine/Graphics/Texture/PostEffect/Glow.hpp>

namespace burn {

	class RenderTarget;

	class Sprite;
	class Rectangle;
	class Label;

	class Model;
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
			LIGHTING,    ///< Output lighting
			UNSHADED	///< Output unshaded colors/materials
		};

	public:

		/**
		 * @brief Creates render textures
		 */
		Renderer();

		/**
		 * @brief Prepare for rendering. Adjust render textures and framebuffers
		 *
		 * @param targetDimensions dimensions of the final render target
		 */
		bool create(const Vector2ui& resolution);

		void clear();

		/**
		 * @brief Put the render result into target
		 *
		 * @param target final render target
		 * @param output The output that is used
		 */
		void finalize(	const RenderTarget& target);

		/**
		 * @brief Render a GuiNode into a render target (e.g. a Window)
		 *
		 * @param node GUI node
		 * @param target RenderTarget to render into
		 */
		void renderGuiNode(const Sprite& node);
		void renderGuiNode(const Rectangle& node);
		void renderGuiNode(const Label& node);

		/**
		 * @brief Render a Scene
		 *
		 * @param scene Scene to render
		 * @param camera Camera to use
		 */
		void renderScene(	const Scene& scene,
							const Camera& camera);

		void setGlowEnabled(bool enabled);

		void setOutput(const Output& output);

	private:

		/**
		 * @brief Render a Model
		 */
		void renderModel(	const Model& model,
							const Camera& camera);

		/**
		 * @brief Render lighting based on current g-buffer data
		 */
		void renderPointLight(	const PointLight& pointLight,
								const Vector3f& cameraPosition);

		/**
		 * @brief Render lighting based on current g-buffer data
		 *
		 * @param scene Contains possible shadow casters
		 * @param focus The focus of the scene. Usually the camera position.
		 * Shadows will have highest resolution here
		 */
		void renderDirectionalLight(const DirectionalLight& directionalLight,
									const Vector3f& cameraPosition,
									const Scene& scene,
									const Vector3f& focus);

		/**
		 * @brief Render lighting based on current g-buffer data
		 */
		void renderSpotLight(	const SpotLight& spotLight,
								const Vector3f& cameraPosition,
								const Scene& scene);

		/**
		 * @brief Render a fullscreen quad with texture coords
		 */
		void renderLighting(const Shader& shader);

	private:
		Vector2ui m_resolution;

		Output m_output;	///< Defines the type of the final output
		// Final image holder:
		Framebuffer m_finalBuffer;
		Texture2D m_finalTexture;
		// Post Effects:
		Glow m_glow;
		bool m_isGlowEnabled;

		// Gui framebuffer:
		Framebuffer m_guiBuffer;
		Texture2D m_guiTexture;
		// G-Buffer:
		Framebuffer m_gBuffer;    ///< Framebuffer storing geometrical information
		Texture2D m_diffuseTexture;
		Texture2D m_normalTexture;
		Texture2D m_positionTexture;
		Texture2D m_unshadedTexture;	///< Holds unshaded colors (eg. reflection)
		// Light rendering:
		Framebuffer m_lightingBuffer;    ///< Framebuffer used for light rendering
		Texture2D m_diffuseLighting, m_specularLighting;
		// Color buffer for billboards, etc.
		Framebuffer m_2DMaterialbuffer;
		Texture2D m_2DMaterialTexture;

		CascadedShadowMap m_cascadedShadowMap;    ///< Used by directional lights
		ShadowMap m_shadowMap;    ///< Used by spot lights
		// Helper:
		VertexBuffer m_fullscreenQuadBuffer;
		VertexArray m_fullscreenQuadVertexArray;
	};

} /* namespace burn */

#endif /* RENDERER_HPP_ */
