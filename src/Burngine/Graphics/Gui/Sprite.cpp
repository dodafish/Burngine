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

#include <Burngine/Graphics/Gui/Sprite.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>

namespace burn {

	Sprite::Sprite() {

		// Setup vertex buffer

		// Position data
		static Vector3f position[] = {
		Vector3f(0.f, 0.f, 0.f),
		Vector3f(1.f, 0.f, 0.f),
		Vector3f(0.f, 1.f, 0.f),
		Vector3f(1.f, 1.f, 0.f) };

		// UV data
		static Vector2f uv[] = {
		Vector2f(0.f, 1.f),
		Vector2f(1.f, 1.f),
		Vector2f(0.f, 0.f),
		Vector2f(1.f, 0.f) };

		// Add data to vertex buffer strided
		for(int i = 0; i != 4; ++i){
			m_vertexBuffer.addData(&position[i], sizeof(Vector3f));
			m_vertexBuffer.addData(&uv[i], sizeof(Vector2f));
		}

	}

	void Sprite::setTexture(const Texture2D& texture,
							bool fitDimensions) {
		m_texture = texture;
		if(fitDimensions)
			setDimensions(Vector2f(m_texture.getDimensions()));
	}

	void Sprite::render(const Matrix4f& view,
						const Matrix4f& projection) const {

		// We must have a loaded texture to render
		if(!m_texture.isLoaded())
			return;

		// We need an OpenGL context
		ensureContext();
		// Our data has to be uploaded
		ensureUpdatedVertexArray();

		// Create Transformable that takes Rectangle's dimensions into account
		Transformable2D t;
		t.setPosition(getPosition());
		t.setRotation(getRotation());
		t.setScale(getScale() * getDimensions());

		// Setup shader
		const Shader& shader = BurnShaders::getShader(BurnShaders::TEXTURE);
		shader.resetTextureUnitCounter();
		shader.setUniform("gModelMatrix", t.getModelMatrix());
		shader.setUniform("gViewMatrix", view);
		shader.setUniform("gProjectionMatrix", projection);
		shader.setUniform("gColor", getColor());
		shader.bindTexture("gTextureSampler", m_texture);
		shader.activate();

		// Render
		m_vertexArray.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		m_vertexArray.unbind();

	}

	void Sprite::render(const Shader& shader) const {

		// We need an OpenGL context
		ensureContext();
		// Our data has to be uploaded
		ensureUpdatedVertexArray();

		// Use shader
		shader.activate();

		// Render
		m_vertexArray.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		m_vertexArray.unbind();

	}

	void Sprite::ensureUpdatedVertexArray() const {

		// Update vertex array if necessary
		if(m_vertexArray.needsUpdate()){

			// Setup vertex array
			m_vertexArray.bind();

			// Enable arrays
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(2);

			// Link vertex buffers
			m_vertexBuffer.bind();
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f), (void*)0);
			glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vector3f) + sizeof(Vector2f),
									(void*)sizeof(Vector3f));

			// "Save" vertex array
			m_vertexArray.unbind();
			m_vertexArray.setUpdated();
		}

	}

} /* namespace burn */
