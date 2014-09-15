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
		updateVertexData();
	}

	void Sprite::setTexture(const Texture& texture,
							bool fitDimensions) {
		m_texture = texture;
		if(fitDimensions)
			setDimensions(Vector2f(m_texture.getDimensions()));
	}

	void Sprite::render(const Matrix4f& view,
						const Matrix4f& projection) const {

		if(!m_texture.isLoaded())
			return;

		ensureContext();

		if(m_vertexArray.needsUpdate()){

			m_vertexArray.bind();
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(2);
			m_vertexBuffer.bind();
			glVertexAttribPointer(	0,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector2f),
									(void*)0);
			glVertexAttribPointer(	2,
									2,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector2f),
									(void*)sizeof(Vector3f));
			m_vertexArray.unbind();

			m_vertexArray.setUpdated();
		}

		const Shader& shader = BurnShaders::getShader(BurnShaders::TEXTURE);
		shader.resetTextureUnitCounter();
		shader.setUniform("gModelMatrix", getModelMatrix());
		shader.setUniform("gViewMatrix", view);
		shader.setUniform("gProjectionMatrix", projection);
		shader.setUniform("gColor", m_color);
		shader.bindTexture("gTextureSampler", m_texture);
		shader.activate();

		m_vertexArray.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		m_vertexArray.unbind();

	}

	void Sprite::updateVertexData() {

		// Create data arrays
		Vector3f position[] = {
		Vector3f(0.f),
		Vector3f(m_dimensions.x, 0.f, 0.f),
		Vector3f(0.f, m_dimensions.y, 0.f),
		Vector3f(m_dimensions.x, m_dimensions.y, 0.f) };

		Vector2f uv[] = {
		Vector2f(0.f, 1.f),
		Vector2f(1.f, 1.f),
		Vector2f(0.f, 0.f),
		Vector2f(1.f, 0.f) };

		// Add data to VBO
		m_vertexBuffer.reset();
		for(int i = 0; i != 4; ++i){
			m_vertexBuffer.addData(&position[i], sizeof(Vector3f));
			m_vertexBuffer.addData(&uv[i], sizeof(Vector2f));
		}

	}

} /* namespace burn */
