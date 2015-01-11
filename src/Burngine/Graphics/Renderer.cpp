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
#include <Burngine/Graphics/Scene/Model.hpp>
#include <Burngine/Graphics/Scene/PointLight.hpp>
#include <Burngine/Graphics/Scene/DirectionalLight.hpp>
#include <Burngine/Graphics/Scene/SpotLight.hpp>
#include <Burngine/Graphics/Scene/Camera.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Graphics/Gui/Sprite.hpp>
#include <Burngine/Graphics/Gui/Rectangle.hpp>
#include <Burngine/Graphics/Gui/Label.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>
#include <Burngine/System/RotationUtil.hpp>

// Shadowmap resolution
#define HIGH 0
#define MID 1
#define LOW 2

namespace burn {

	Renderer::Renderer() :
	m_output(FINAL),
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
		m_cascadedShadowMap.create(1024);
		m_shadowMap.create(1024);

	}

	void Renderer::setGlowEnabled(bool enabled) {
		m_isGlowEnabled = enabled;
	}

	void Renderer::setOutput(const Output& output) {
		m_output = output;
	}

	bool Renderer::create(const Vector2ui& resolution) {

		// Don't recreate with same resolution
		if(m_resolution == resolution)
			return true;

		///////////////////////////////////////////////////////////////////////
		// Final Output Buffer:
		if(!m_finalTexture.loadFromData(resolution, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0)){
			burnWarn("Failed to create texture (final result).");
			return false;
		}
		m_finalTexture.setFiltering(BaseTexture::MAG_NEAREST, BaseTexture::MIN_NEAREST);
		if(!m_finalBuffer.create(resolution, false, m_finalTexture)){
			burnWarn("Failed to create buffer (final result).");
			return false;
		}

		///////////////////////////////////////////////////////////////////////
		// Glow Post Effect
		if(!m_glow.create(m_finalTexture)){
			burnWarn("Failed to setup Glow Post Effect.");
			return false;
		}

		///////////////////////////////////////////////////////////////////////
		// GUI buffer:
		if(!m_guiTexture.loadFromData(resolution, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0)){
			burnWarn("Failed to create texture (GUI).");
			return false;
		}
		m_guiTexture.setFiltering(BaseTexture::MAG_NEAREST, BaseTexture::MIN_NEAREST);
		if(!m_guiBuffer.create(resolution, false, m_guiTexture)){
			burnWarn("Failed to create buffer (GUI).");
			return false;
		}

		///////////////////////////////////////////////////////////////////////
		// 2D-Material buffer:
		if(!m_2DMaterialTexture.loadFromData(resolution, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0)){
			burnWarn("Failed to create texture (2D-Material).");
			return false;
		}
		m_2DMaterialTexture.setFiltering(BaseTexture::MAG_NEAREST, BaseTexture::MIN_NEAREST);
		if(!m_2DMaterialbuffer.create(resolution, false, m_2DMaterialTexture)){
			burnWarn("Failed to create buffer (2D-Material).");
			return false;
		}

		///////////////////////////////////////////////////////////////////////
		// G-Buffers:
		//
		// RGBA: Diffuse colors
		if(!m_diffuseTexture.loadFromData(resolution, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0)){
			burnWarn("Failed to create texture (G-Buffer diffuse color).");
			return false;
		}
		m_diffuseTexture.setFiltering(BaseTexture::MAG_NEAREST, BaseTexture::MIN_NEAREST);
		// RGBA: Unshaded colors
		if(!m_unshadedTexture.loadFromData(resolution, GL_RGBA8, GL_RGBA, GL_UNSIGNED_BYTE, 0)){
			burnWarn("Failed to create texture (G-Buffer diffuse color).");
			return false;
		}
		m_unshadedTexture.setFiltering(BaseTexture::MAG_NEAREST, BaseTexture::MIN_NEAREST);
		// RGB: Normals
		if(!m_normalTexture.loadFromData(resolution, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 0)){
			burnWarn("Failed to create texture (G-Buffer normals).");
			return false;
		}
		m_normalTexture.setFiltering(BaseTexture::MAG_NEAREST, BaseTexture::MIN_NEAREST);
		// RGB: World space positions
		if(!m_positionTexture.loadFromData(resolution, GL_RGB16F, GL_RGB, GL_FLOAT, 0)){
			burnWarn("Failed to create texture (G-Buffer world space coords).");
			return false;
		}
		m_positionTexture.setFiltering(BaseTexture::MAG_NEAREST, BaseTexture::MIN_NEAREST);

		// G-Buffer framebuffer
		if(!m_gBuffer.create(resolution, true, m_diffuseTexture)){
			burnWarn("Cannot create G-Buffer!");
			return false;
		}
		// Attach other textures
		if(!m_gBuffer.attachTexture(m_normalTexture, 1)){
			burnWarn("Cannot attach normal texture!");
			return false;
		}
		if(!m_gBuffer.attachTexture(m_positionTexture, 2)){
			burnWarn("Cannot attach position texture!");
			return false;
		}
		if(!m_gBuffer.attachTexture(m_unshadedTexture, 3)){
			burnWarn("Cannot attach unshaded texture!");
			return false;
		}

		///////////////////////////////////////////////////////////////////////
		// Lighting Buffer:
		//
		// RGB: Diffuse lighting
		if(!m_diffuseLighting.loadFromData(resolution, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 0)){
			burnWarn("Failed to create texture (G-Buffer diffuse lighting).");
			return false;
		}
		m_diffuseLighting.setFiltering(BaseTexture::MAG_NEAREST, BaseTexture::MIN_NEAREST);
		// RGB: Specular lighting
		if(!m_specularLighting.loadFromData(resolution, GL_RGB8, GL_RGB, GL_UNSIGNED_BYTE, 0)){
			burnWarn("Failed to create texture (G-Buffer specular lighting).");
			return false;
		}
		m_specularLighting.setFiltering(BaseTexture::MAG_NEAREST, BaseTexture::MIN_NEAREST);
		if(!m_lightingBuffer.create(resolution, false, m_diffuseLighting)){
			burnWarn("Failed to create buffer (Lighting).");
			return false;
		}
		if(!m_lightingBuffer.attachTexture(m_specularLighting, 1)){
			burnWarn("Cannot attach specular lighting texture!");
			return false;
		}

		// All buffers setup
		return true;
	}

	void Renderer::clear() {
		// Clear render textures
		m_finalBuffer.clear();
		m_gBuffer.clear();
		m_lightingBuffer.clear();
		m_guiBuffer.clear();
		m_2DMaterialbuffer.clear();
	}

	void Renderer::finalize(const RenderTarget& target) {

		ensureContext();

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_CULL_FACE);

		// Output with a simple sprite
		static Sprite sprite;
		sprite.setDimensions(Vector2f(m_diffuseTexture.getDimensions()));

		// Render final texture
		if(m_output == FINAL){

			if(m_finalBuffer.prepare()){

				// Apply lighting
				glBlendFunc(GL_ONE,
				GL_ZERO);    // Overwrite

				// Setup model matrix to position vertices properly
				Transformable2D t;
				t.setScale(sprite.getDimensions());

				const Shader& shader = BurnShaders::getShader(BurnShaders::FINALIZE);
				shader.resetTextureUnitCounter();
				shader.setUniform("gModelMatrix", t.getModelMatrix());
				shader.setUniform("gProjectionMatrix", m_finalBuffer.getOrtho());
				shader.bindTexture("gColorSampler", m_diffuseTexture);
				shader.bindTexture("gDiffuseSampler", m_diffuseLighting);
				shader.bindTexture("gSpecularSampler", m_specularLighting);

				sprite.render(shader);

				// Apply Post Effects
				if(m_isGlowEnabled){
					m_glow.passInput(m_finalTexture);
					m_glow.getOutput(m_finalBuffer);
				}

				// Apply 2D Objects
				glBlendFunc(GL_SRC_ALPHA,
				GL_ONE_MINUS_SRC_ALPHA);
				sprite.setTexture(m_2DMaterialTexture);
				sprite.render(Matrix4f(1.f), m_finalBuffer.getOrtho());

				// Apply GUI
				glBlendFunc(GL_SRC_ALPHA,
				GL_ONE_MINUS_SRC_ALPHA);
				sprite.setTexture(m_guiTexture);
				sprite.render(Matrix4f(1.f), m_finalBuffer.getOrtho());

			}

		}

		if(target.prepare()){

			if(m_output == FINAL){
				glBlendFunc(GL_ONE,
				GL_ZERO);    // Overwrite
				sprite.setTexture(m_finalTexture);
				sprite.render(Matrix4f(1.f), target.getOrtho());
				glBlendFunc(GL_SRC_ALPHA,
				GL_ONE_MINUS_SRC_ALPHA);
				return;
			}else if(m_output == DIFFUSE){
				sprite.setTexture(m_diffuseTexture);
				sprite.render(Matrix4f(1.f), target.getOrtho());

				// Apply 2D Objects
				glBlendFunc(GL_SRC_ALPHA,
				GL_ONE_MINUS_SRC_ALPHA);
				sprite.setTexture(m_2DMaterialTexture);
				sprite.render(Matrix4f(1.f), target.getOrtho());
				return;
			}else if(m_output == POSITION)
				sprite.setTexture(m_positionTexture);
			else if(m_output == LIGHTING)
				sprite.setTexture(m_diffuseLighting);
			else
				// output == NORMAL
				sprite.setTexture(m_normalTexture);

			sprite.render(Matrix4f(1.f), target.getOrtho());

			glBlendFunc(GL_SRC_ALPHA,
			GL_ONE_MINUS_SRC_ALPHA);

		}
	}

	void Renderer::renderScene(	const Scene& scene,
								const Camera& camera) {

		// Calculate camera matrices
		Matrix4f view = glm::lookAt(camera.getPosition(),
									camera.getPosition()
									+ Vector3f(camera.getRotation().asMatrix()
									* Vector4f(0.f, 0.f, -1.f, 1.f)),
									Vector3f(0.f, 1.f, 0.f));

		Matrix4f projection = glm::perspective<float>(	camera.getFov(),
														camera.getAspectRatio(),
														0.01f,
														10000.f);

		/*
		 * Fill G-Buffers:
		 */
		Skybox* skybox = scene.getSkybox();
		if(skybox != NULL){
			if(m_gBuffer.prepare()){
				ensureContext();
				glDisable(GL_CULL_FACE);
				skybox->render(camera.getPosition(), view, projection);
			}
		}

		const std::vector<Model*>& models = scene.getModels();
		for(size_t i = 0; i < models.size(); ++i)
			renderModel(*(models[i]), camera);

		/*
		 * Render Lighting:
		 */
		if(m_output == FINAL || m_output == LIGHTING){
			const std::vector<DirectionalLight*> directionalLights = scene.getDirectionalLights();
			for(size_t i = 0; i < directionalLights.size(); ++i){

				// Focus is 25 units into the viewing direction
				Vector3f focus = camera.getPosition();
				Vector3f dir = Vector3f(camera.getRotation().asMatrix() * Vector4f(0.f, 0.f, -1.f, 1.f));
				focus += 10.f * glm::normalize(dir);

				renderDirectionalLight(*(directionalLights[i]), camera.getPosition(), scene, focus);
			}

			const std::vector<SpotLight*> spotLights = scene.getSpotLights();
			for(size_t i = 0; i < spotLights.size(); ++i)
				renderSpotLight(*(spotLights[i]), camera.getPosition(), scene);

			const std::vector<PointLight*> pointLights = scene.getPointLights();
			for(size_t i = 0; i < pointLights.size(); ++i)
				renderPointLight(*(pointLights[i]), camera.getPosition());
		}

		/*
		 * Render diffuse objects
		 */
		if(m_output == FINAL || m_output == DIFFUSE){

			if(m_2DMaterialbuffer.prepare()){

				const std::vector<Billboard*>& billboards = scene.getBillboards();
				for(size_t i = 0; i < billboards.size(); ++i){

					Rotation rot = RotationUtil::RotationBetweenVectors(Vector3f(0.f, 0.f, 1.f),
																		camera.getPosition()
																		- billboards[i]->getPosition());

					billboards[i]->setRotation(rot);
					billboards[i]->render(view, projection);
				}
			}
		}

	}

	void Renderer::renderGuiNode(const Sprite& node) {

		ensureContext();

		// OpenGL flags
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		if(m_guiBuffer.prepare()){
			// Render the node with the
			node.render(Matrix4f(1.f), m_guiBuffer.getOrtho());
		}

	}
	void Renderer::renderGuiNode(const Rectangle& node) {

		ensureContext();

		// OpenGL flags
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		if(m_guiBuffer.prepare()){
			// Render the node with the
			node.render(Matrix4f(1.f), m_guiBuffer.getOrtho());
		}

	}
	void Renderer::renderGuiNode(const Label& node) {

		ensureContext();

		// OpenGL flags
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA);
		glDisable(GL_DEPTH_TEST);

		if(m_guiBuffer.prepare()){
			// Render the node with the
			node.render(Matrix4f(1.f), m_guiBuffer.getOrtho());
		}

	}

	void Renderer::renderModel(	const Model& model,
								const Camera& camera) {

		ensureContext();

		// OpenGL flags
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA,
		GL_ONE_MINUS_SRC_ALPHA);
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
			model.render(view, projection);
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

			glBlendFunc(GL_ONE,
			GL_ONE);    // Add
			renderLighting(shader);

		}

	}

	void Renderer::renderDirectionalLight(	const DirectionalLight& directionalLight,
											const Vector3f& cameraPosition,
											const Scene& scene,
											const Vector3f& focus) {

		ensureContext();

		m_cascadedShadowMap.render(directionalLight, scene.getModels(), focus);

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

			glBlendFunc(GL_ONE,
			GL_ONE);    // Add
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
											dir == Vector3f(0.f, -1.f, 0.f) ? 	Vector3f(1.f, 0.f, 0.f) :
																				Vector3f(0.f, 1.f, 0.f));

		float zFar = glm::sqrt(spotLight.getIntensity() / 0.01f);
		//zFar = 100.f;

		Matrix4f projMatrix = glm::perspective<float>(spotLight.getConeAngle() * 2.f, 1.f, 0.0001f, zFar);

		m_shadowMap.render(scene.getModels(), viewMatrix, projMatrix, true);

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

			glBlendFunc(GL_ONE,
			GL_ONE);    // Add
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
		glDrawArrays( GL_TRIANGLE_STRIP, 0, 4);
		m_fullscreenQuadVertexArray.unbind();

	}

} /* namespace burn */
