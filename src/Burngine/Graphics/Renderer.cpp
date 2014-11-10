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

// Shadowmap resolution
#define HIGH 0
#define MID 1
#define LOW 2

namespace burn {

	Renderer::Renderer() :
	m_isGlowEnabled(true) {

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
		m_cascadedShadowMap.create(2048);
		m_shadowMap.create(2048);

	}

	void Renderer::setGlowEnabled(bool enabled) {
		m_isGlowEnabled = enabled;
	}

	void Renderer::prepare(const Vector2ui& targetDimensions) {

		if(m_finalTexture.getDimensions() != targetDimensions){
			m_finalTexture.loadFromData(targetDimensions, Texture::RGBA, Texture::DATA_RGBA, 0);
		}
		if(m_finalBuffer.getDimensions() != targetDimensions){
			m_finalBuffer.create(targetDimensions, false, m_finalTexture);
		}

		if(m_guiTexture.getDimensions() != targetDimensions){
			m_guiTexture.loadFromData(targetDimensions, Texture::RGBA, Texture::DATA_RGBA, 0);
		}
		if(m_guiBuffer.getDimensions() != targetDimensions){
			m_guiBuffer.create(targetDimensions, false, m_guiTexture);
		}

		// Adjust gbuffer textures if necessary
		if(m_diffuseTexture.getDimensions() != targetDimensions){
			// RGBA: Diffuse colors
			m_diffuseTexture.loadFromData(targetDimensions, Texture::RGBA, Texture::DATA_RGBA, 0);
			// RGB: Normals
			m_normalTexture.loadFromData(targetDimensions, Texture::RGB, Texture::DATA_RGB, 0);
			// RGB: World space positions
			m_positionTexture.loadFromData(targetDimensions, Texture::RGB16F, Texture::DATA_RGB, 0);
			// RGB: Diffuse lighting
			m_diffuseLighting.loadFromData(targetDimensions, Texture::RGB, Texture::DATA_RGB, 0);
			// RGB: Specular lighting
			m_specularLighting.loadFromData(targetDimensions, Texture::RGB, Texture::DATA_RGB, 0);
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
			if(!m_lightingBuffer.create(targetDimensions, false, m_diffuseLighting)){
				burnErr("Cannot recreate Lighting-Buffer!");
			}
			m_lightingBuffer.attachTexture(m_specularLighting, 1);
		}

		// Clear render textures
		m_finalBuffer.clear();
		m_gBuffer.clear();
		m_lightingBuffer.clear();
		m_guiBuffer.clear();
	}

	void Renderer::finalize(const RenderTarget& target,
							const Output& output) {

		ensureContext();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// Output with a simple sprite
		Sprite sprite;
		sprite.setDimensions(Vector2f(m_diffuseTexture.getDimensions()));

		// Render final texture
		if(output == FINAL){

			if(m_finalBuffer.prepare()){

				// Apply lighting
				glBlendFunc(GL_ONE, GL_ZERO);    // Overwrite

				const Shader& shader = BurnShaders::getShader(BurnShaders::FINALIZE);
				shader.resetTextureUnitCounter();
				shader.setUniform("gProjectionMatrix", m_finalBuffer.getOrtho());
				shader.bindTexture("gColorSampler", m_diffuseTexture);
				shader.bindTexture("gDiffuseSampler", m_diffuseLighting);
				shader.bindTexture("gSpecularSampler", m_specularLighting);

				sprite.render(shader);

				// Apply Post Effects
				if(m_isGlowEnabled)
					m_glow.apply(m_finalTexture, &m_finalBuffer);

				// Apply GUI
				glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
				sprite.setTexture(m_guiTexture);
				sprite.render(Matrix4f(1.f), m_finalBuffer.getOrtho());

			}

		}

		if(target.prepare()){

			if(output == FINAL){
				glBlendFunc(GL_ONE, GL_ZERO);    // Overwrite
				sprite.setTexture(m_finalTexture);
				sprite.render(Matrix4f(1.f), target.getOrtho());
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
		for(size_t i = 0; i < directionalLights.size(); ++i){

			// Focus is 25 units into the viewing direction
			Vector3f focus = camera.getPosition();
			Vector3f dir = Vector3f(camera.getRotation().asMatrix() * Vector4f(0.f, 0.f, -1.f, 1.f));
			focus += 25.f * glm::normalize(dir);

			renderDirectionalLight(*(directionalLights[i]), camera.getPosition(), scene, focus);
		}

		const std::vector<SpotLight*> spotLights = scene.getSpotLights();
		for(size_t i = 0; i < spotLights.size(); ++i)
			renderSpotLight(*(spotLights[i]), camera.getPosition(), scene);

		const std::vector<PointLight*> pointLights = scene.getPointLights();
		for(size_t i = 0; i < pointLights.size(); ++i)
			renderPointLight(*(pointLights[i]), camera.getPosition());

	}

	void Renderer::renderGuiNode(const GuiNode& node) {

		ensureContext();

		// OpenGL flags
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		if(m_guiBuffer.prepare()){
			// Render the node with the
			node.render(Matrix4f(1.f), m_guiBuffer.getOrtho());
		}

	}

	void Renderer::renderSceneNode(	const SceneNode& node,
									const Camera& camera) {

		ensureContext();

		// OpenGL flags
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_BACK);

		if(m_gBuffer.prepare()){

			// Calculate the view matrix
			Matrix4f view = glm::lookAt(camera.getPosition(),
										camera.getPosition()
										+ Vector3f(camera.getRotation().asMatrix()
										* Vector4f(0.f, 0.f, -1.f, 1.f)),
										Vector3f(0.f, 1.f, 0.f));
			// Calculate the projection matrix
			Matrix4f projection = glm::perspective<float>(	camera.getFov(),
															camera.getAspectRatio(),
															0.01f,
															10000.f);

			// Render the node with the matrices
			node.render(view, projection);
		}

	}

	void Renderer::renderPointLight(const PointLight& pointLight,
									const Vector3f& cameraPosition) {

		ensureContext();

		if(m_lightingBuffer.prepare()){

			const Shader& shader = BurnShaders::getShader(BurnShaders::POINT_LIGHT);
			shader.resetTextureUnitCounter();
			shader.setUniform("gCameraPosition", cameraPosition);
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
											const Vector3f& cameraPosition,
											const Scene& scene,
											const Vector3f& focus) {

		ensureContext();

		m_cascadedShadowMap.render(directionalLight, scene.getSceneNodes(), focus);

		if(m_lightingBuffer.prepare()){

			// Render the lighting
			const Shader& shader = BurnShaders::getShader(BurnShaders::DIRECTIONAL_LIGHT);
			shader.resetTextureUnitCounter();
			shader.setUniform("gCameraPosition", cameraPosition);
			shader.setUniform("gShadowViewMatrix", m_cascadedShadowMap.getUsedViewMatrix());
			shader.setUniform(	"gShadowProjectionMatrix_WIDE",
								m_cascadedShadowMap.getUsedProjectionMatrix(CascadedShadowMap::WIDE));
			shader.setUniform(	"gShadowProjectionMatrix_MEDIUM",
								m_cascadedShadowMap.getUsedProjectionMatrix(CascadedShadowMap::MEDIUM));
			shader.setUniform(	"gShadowProjectionMatrix_SMALL",
								m_cascadedShadowMap.getUsedProjectionMatrix(CascadedShadowMap::SMALL));
			shader.setUniform("gLightDirection", directionalLight.getDirection());
			shader.setUniform("gLightColor", directionalLight.getColor());
			shader.setUniform("gLightIntensity", directionalLight.getIntensity());
			shader.bindTexture("gNormalSampler", m_normalTexture);
			shader.bindTexture("gPositionSampler", m_positionTexture);
			shader.bindTexture(	"gShadowMapSampler_WIDE",
								m_cascadedShadowMap.getShadowMap(CascadedShadowMap::WIDE));
			shader.bindTexture(	"gShadowMapSampler_MEDIUM",
								m_cascadedShadowMap.getShadowMap(CascadedShadowMap::MEDIUM));
			shader.bindTexture(	"gShadowMapSampler_SMALL",
								m_cascadedShadowMap.getShadowMap(CascadedShadowMap::SMALL));

			glBlendFunc(GL_ONE, GL_ONE);    // Add
			renderLighting(shader);

		}

	}

	void Renderer::renderSpotLight(	const SpotLight& spotLight,
									const Vector3f& cameraPosition,
									const Scene& scene) {

		ensureContext();
		glDisable(GL_CULL_FACE);

		Vector3f dir = spotLight.getDirection();

		Matrix4f viewMatrix = glm::lookAt(	spotLight.getPosition(),
											spotLight.getPosition() - dir,
											dir == Vector3f(0.f, -1.f, 0.f) ?
											Vector3f(1.f, 0.f, 0.f) : Vector3f(0.f, 1.f, 0.f));

		float zFar = glm::sqrt(spotLight.getIntensity() / 0.01f);
		//zFar = 100.f;

		Matrix4f projMatrix = glm::perspective<float>(spotLight.getConeAngle() * 2.f, 1.f, 0.0001f, zFar);

		m_shadowMap.render(scene.getSceneNodes(), viewMatrix, projMatrix, true);

		if(m_lightingBuffer.prepare()){

			float lightConeCosine = std::cos(spotLight.getConeAngle() / (180.f / 3.1415f));

			const Shader& shader = BurnShaders::getShader(BurnShaders::SPOT_LIGHT);
			shader.resetTextureUnitCounter();
			shader.setUniform("gCameraPosition", cameraPosition);
			shader.setUniform("gLightDirection", spotLight.getDirection());
			shader.setUniform("gLightPosition", spotLight.getPosition());
			shader.setUniform("gLightColor", spotLight.getColor());
			shader.setUniform("gLightIntensity", spotLight.getIntensity());
			shader.setUniform("gLightConeCosine", lightConeCosine);
			shader.bindTexture("gPositionSampler", m_positionTexture);
			shader.bindTexture("gNormalSampler", m_normalTexture);

			shader.setUniform("gShadowViewMatrix", viewMatrix);
			shader.setUniform("gShadowProjectionMatrix", projMatrix);
			shader.bindTexture("gShadowMapSampler", m_shadowMap.getTexture());

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
			glVertexAttribPointer(0, 2,
			GL_FLOAT,
									GL_FALSE, sizeof(Vector2f) + sizeof(Vector2f), (void*)0);
			glVertexAttribPointer(1, 2,
			GL_FLOAT,
									GL_FALSE, sizeof(Vector2f) + sizeof(Vector2f), (void*)sizeof(Vector2f));

			m_fullscreenQuadVertexArray.unbind();
			m_fullscreenQuadVertexArray.setUpdated();
		}

		shader.activate();
		m_fullscreenQuadVertexArray.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		m_fullscreenQuadVertexArray.unbind();

	}

} /* namespace burn */
