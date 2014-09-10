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
#include <Burngine/Graphics/Scene/Camera.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Graphics/Gui/Sprite.hpp>

namespace burn {

	Renderer::Renderer() {

		// Create the fullscreen quad buffer

		Vector2f positions[4] = {
		Vector2f(0.f, 0.f),
		Vector2f(1.f, 0.f),
		Vector2f(0.f, 1.f),
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

	}

	void Renderer::prepare(const Vector2ui& targetDimensions) {

		// Adjust gbuffer textures if necessary
		if(m_diffuseTexture.getDimensions() != targetDimensions){
			// Empty RGBA texture
			m_diffuseTexture.loadFromData(targetDimensions, 32, 0);
			// Empty RGB texture
			m_normalTexture.loadFromData(targetDimensions, 24, 0);
			// Empty RGB texture with floats
			m_positionTexture.loadFromData(	targetDimensions,
											24,
											0,
											Texture::FLOAT);
			// Empty RGBA texture
			m_diffuseLighting.loadFromData(targetDimensions, 32, 0);
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

			if(output == FINAL)
				sprite.setTexture(m_diffuseTexture);
			else if(output == DIFFUSE)
				sprite.setTexture(m_diffuseTexture);
			else if(output == POSITION)
				sprite.setTexture(m_positionTexture);
			else
				// output == NORMAL
				sprite.setTexture(m_normalTexture);

			sprite.render(Matrix4f(1.f), target.getOrtho());

		}
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
