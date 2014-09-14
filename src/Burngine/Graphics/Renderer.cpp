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

#include <Burngine/Graphics/Renderer.hpp>
#include <Burngine/Window/Window.hpp>
#include <Burngine/Graphics/Gui/GuiNode.hpp>
#include <Burngine/Graphics/Scene/SceneNode.hpp>
#include <Burngine/Graphics/Scene/PointLight.hpp>
#include <Burngine/Graphics/Scene/DirectionalLight.hpp>
#include <Burngine/Graphics/Scene/SpotLight.hpp>
#include <Burngine/Graphics/Scene/Camera.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Graphics/Gui/Sprite.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>

namespace burn {

	Renderer::Renderer() {

		// Create the fullscreen quad buffer

		Vector2f positions[4] = {
		Vector2f(-1.f, -1.f),
		Vector2f(1.f, -1.f),
		Vector2f(-1.f, 1.f),
		Vector2f(1.f, 1.f) };

		Vector2f uvCoords[4] = {
		Vector2f(0.f, 0.f),
		Vector2f(1.f, 0.f),
		Vector2f(0.f, 1.f),
		Vector2f(1.f, 1.f) };

		for(int i = 0; i != 4; ++i){
			m_fullscreenQuadBuffer.addData(&positions[i], sizeof(Vector2f));
			m_fullscreenQuadBuffer.addData(&uvCoords[i], sizeof(Vector2f));
		}

		// Shadow maps:

		// RG: VSM
		m_shadowMap.loadFromData(	Vector2ui(1024, 1024),
									Texture::RG16F,
									Texture::DATA_RG,
									0);

		if(!m_shadowMapBuffer.create(	Vector2ui(1024, 1024),
										true,
										m_shadowMap)){
			burnErr("Cannot create simple shadow map.");
		}

	}

	void Renderer::prepare(const Vector2ui& targetDimensions) {

		// Adjust gbuffer textures if necessary
		if(m_diffuseTexture.getDimensions() != targetDimensions){
			// RGBA: Diffuse colors
			m_diffuseTexture.loadFromData(	targetDimensions,
											Texture::RGBA,
											Texture::DATA_RGBA,
											0);
			// RGB: Normals
			m_normalTexture.loadFromData(	targetDimensions,
											Texture::RGB,
											Texture::DATA_RGB,
											0);
			// RGB: World space positions
			m_positionTexture.loadFromData(	targetDimensions,
											Texture::RGB16F,
											Texture::DATA_RGB,
											0);
			// RGB: Diffuse lighting
			m_diffuseLighting.loadFromData(	targetDimensions,
											Texture::RGB,
											Texture::DATA_RGB,
											0);
		}

		// Adjust framebuffer if necessary
		if(m_gBuffer.getDimensions() != targetDimensions){
			if(!m_gBuffer.create(targetDimensions, true, m_diffuseTexture))
				burnErr("Cannot recreate G-Buffer!");
			// Attach other textures
			if(!m_gBuffer.attachTexture(m_normalTexture, 1))
				burnErr("Cannot attach normal texture!");
			if(!m_gBuffer.attachTexture(m_positionTexture, 2))
				burnErr("Cannot attach position texture!");
			/////////////////////////////////////////////////////////////////
			if(!m_lightingBuffer.create(targetDimensions,
										false,
										m_diffuseLighting)){
				burnErr("Cannot recreate Lighting-Buffer!");
			}
		}

		// Clear render textures
		m_gBuffer.clear();
		m_lightingBuffer.clear();
	}

	void Renderer::finalize(const RenderTarget& target,
							const Output& output) {
		if(target.prepare()){
			ensureContext();

			glDisable(GL_DEPTH_TEST);

			// Output with a simple sprite
			Sprite sprite;
			sprite.setDimensions(Vector2f(m_diffuseTexture.getDimensions()));

			if(output == FINAL){
				glBlendFunc(GL_ONE, GL_ZERO);    // Overwrite
				sprite.setTexture(m_diffuseTexture);
				sprite.render(Matrix4f(1.f), target.getOrtho());
				glBlendFunc(GL_ZERO, GL_SRC_COLOR);    // Multiply
				sprite.setTexture(m_diffuseLighting);
				sprite.render(Matrix4f(1.f), target.getOrtho());
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				return;
			}else if(output == DIFFUSE)
				sprite.setTexture(m_diffuseTexture);
			else if(output == POSITION)
				sprite.setTexture(m_positionTexture);
			else if(output == LIGHTING)
				sprite.setTexture(m_diffuseLighting);
			else
				// output == NORMAL
				sprite.setTexture(m_normalTexture);

			sprite.render(Matrix4f(1.f), target.getOrtho());

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		}
	}

	void Renderer::renderScene(	const Scene& scene,
								const Camera& camera) {

		const std::vector<SceneNode*>& sceneNodes = scene.getSceneNodes();
		for(size_t i = 0; i < sceneNodes.size(); ++i)
			renderSceneNode(*(sceneNodes[i]), camera);

		const std::vector<DirectionalLight*> directionalLights = scene.getDirectionalLights();
		for(size_t i = 0; i < directionalLights.size(); ++i)
			renderDirectionalLight(*(directionalLights[i]), scene);

		const std::vector<SpotLight*> spotLights = scene.getSpotLights();
		for(size_t i = 0; i < spotLights.size(); ++i)
			renderSpotLight(*(spotLights[i]));

		const std::vector<PointLight*> pointLights = scene.getPointLights();
		for(size_t i = 0; i < pointLights.size(); ++i)
			renderPointLight(*(pointLights[i]));

	}

	const Texture& Renderer::getShadowMap() const {
		return m_shadowMap;
	}

	void Renderer::renderGuiNode(const GuiNode& node) {

		ensureContext();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(m_gBuffer.prepare()){
			node.render(Matrix4f(1.f), m_gBuffer.getOrtho());
		}

	}

	void Renderer::renderSceneNode(	const SceneNode& node,
									const Camera& camera) {

		ensureContext();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);

		if(m_gBuffer.prepare()){

			node.render(glm::lookAt(camera.getPosition(),
									Vector3f(0.f, 0.f, -0.1f),
									Vector3f(0.f, 1.f, 0.f)),
						glm::perspective<float>(30.f,
												16.f / 9.f,
												0.01f,
												10000.f));
		}

	}

	void Renderer::renderPointLight(const PointLight& pointLight) {

		ensureContext();

		if(m_lightingBuffer.prepare()){

			const Shader& shader = BurnShaders::getShader(BurnShaders::POINT_LIGHT);
			shader.resetTextureUnitCounter();
			shader.setUniform("gLightPosition", pointLight.getPosition());
			shader.setUniform("gLightColor", pointLight.getColor());
			shader.setUniform("gLightIntensity", pointLight.getIntensity());
			shader.bindTexture("gPositionSampler", m_positionTexture);
			shader.bindTexture("gNormalSampler", m_normalTexture);

			glBlendFunc(GL_ONE, GL_ONE);    // Add
			renderLighting(shader);

		}

	}

	void Renderer::renderDirectionalLight(	const DirectionalLight& directionalLight,
											const Scene& scene) {

		ensureContext();

		// Clear shadowmap
		m_shadowMapBuffer.clear();

		// Render shadowmap
		if(m_shadowMapBuffer.prepare()){

			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			// Calculate light's view matrix
			Matrix4f lightView = glm::lookAt(	Vector3f(0.1f),
												Vector3f(0.1f)
												- directionalLight.getDirection(),
												Vector3f(0.f, 1.f, 0.f));

			// Calculate light's projection matrix
			Matrix4f lightProjection = glm::ortho(	-50.f,
													50.f,
													-50.f,
													50.f,
													-500.f,
													500.f);

			// Render all shadow casters
			const std::vector<SceneNode*>& sceneNodes = scene.getSceneNodes();
			for(size_t i = 0; i < sceneNodes.size(); ++i){
				sceneNodes[i]->renderShadowMap(lightView, lightProjection, true);
			}

			if(m_lightingBuffer.prepare()){

				// Render the lighting
				const Shader& shader = BurnShaders::getShader(BurnShaders::DIRECTIONAL_LIGHT);
				shader.resetTextureUnitCounter();
				shader.setUniform("gShadowViewMatrix", lightView);
				shader.setUniform("gShadowProjectionMatrix", lightProjection);
				shader.setUniform(	"gLightDirection",
									directionalLight.getDirection());
				shader.setUniform("gLightColor", directionalLight.getColor());
				shader.setUniform(	"gLightIntensity",
									directionalLight.getIntensity());
				shader.bindTexture("gNormalSampler", m_normalTexture);
				shader.bindTexture("gPositionSampler", m_positionTexture);
				shader.bindTexture("gShadowMapSampler", m_shadowMap);

				glBlendFunc(GL_ONE, GL_ONE);    // Add
				renderLighting(shader);

			}

		}

	}

	void Renderer::renderSpotLight(const SpotLight& spotLight) {

		ensureContext();

		if(m_lightingBuffer.prepare()){

			float lightConeCosine = std::cos(spotLight.getConeAngle()
			/ (180.f / 3.1415f));

			const Shader& shader = BurnShaders::getShader(BurnShaders::SPOT_LIGHT);
			shader.resetTextureUnitCounter();
			shader.setUniform("gLightDirection", spotLight.getDirection());
			shader.setUniform("gLightPosition", spotLight.getPosition());
			shader.setUniform("gLightColor", spotLight.getColor());
			shader.setUniform("gLightIntensity", spotLight.getIntensity());
			shader.setUniform("gLightConeCosine", lightConeCosine);
			shader.bindTexture("gPositionSampler", m_positionTexture);
			shader.bindTexture("gNormalSampler", m_normalTexture);

			glBlendFunc(GL_ONE, GL_ONE);    // Add
			renderLighting(shader);

		}

	}

	void Renderer::renderLighting(const Shader& shader) {
		ensureContext();

		if(m_fullscreenQuadVertexArray.needsUpdate()){
			m_fullscreenQuadVertexArray.bind();
			m_fullscreenQuadBuffer.bind();

			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glVertexAttribPointer(	0,
									2,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector2f) + sizeof(Vector2f),
									(void*)0);
			glVertexAttribPointer(	1,
									2,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector2f) + sizeof(Vector2f),
									(void*)sizeof(Vector2f));

			m_fullscreenQuadVertexArray.unbind();
			m_fullscreenQuadVertexArray.setUpdated();
		}

		shader.activate();
		m_fullscreenQuadVertexArray.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		m_fullscreenQuadVertexArray.unbind();

	}

} /* namespace burn */
