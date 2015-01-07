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

#include <Burngine/Graphics/Scene/Billboard.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>

namespace burn {

	Billboard::Billboard() :
	m_dimensions(1.f),    // Default to 1.f so we can see anything if we forget to set dimensions
	m_color(1.f) {
		updateVertexData();
	}

	void Billboard::render(	const Matrix4f& view,
							const Matrix4f& projection) const {

		// We must have a loaded texture to render
		if(!m_texture.isLoaded())
			return;

		// We need an OpenGL context
		ensureContext();
		// Our data has to be uploaded
		ensureUpdatedVertexArray();

		const Shader& shader = BurnShaders::getShader(BurnShaders::BILLBOARD);
		shader.resetTextureUnitCounter();
		shader.setUniform(	"gModelMatrix",
							getGlobalModelMatrix());
		shader.setUniform(	"gViewMatrix",
							view);
		shader.setUniform(	"gProjectionMatrix",
							projection);
		shader.setUniform(	"gColor",
							m_color);
		shader.bindTexture(	"gTextureSampler",
							m_texture);
		shader.activate();

		m_vertexArray.bind();
		glDrawArrays( 	GL_TRIANGLE_STRIP,
						0,
						4);
		m_vertexArray.unbind();

	}

	void Billboard::setTexture(const Texture2D& texture) {
		m_texture = texture;
	}

	const Texture2D& Billboard::getTexture() const {
		return m_texture;
	}

	void Billboard::updateVertexData() {

		// Create data arrays
		Vector3f position[] = {
		Vector3f(0.f),
		Vector3f(	m_dimensions.x,
					0.f,
					0.f),
		Vector3f(	0.f,
					m_dimensions.y,
					0.f),
		Vector3f(	m_dimensions.x,
					m_dimensions.y,
					0.f) };

		Vector2f uv[] = {
		Vector2f(0.f),
		Vector2f(	1.f,
					0.f),
		Vector2f(	0.f,
					1.f),
		Vector2f(1.f) };

		// Add data to VBO
		m_vertexBuffer.reset();
		for(int i = 0; i != 4; ++i){
			m_vertexBuffer.addData(	&position[i],
									sizeof(Vector3f));
			m_vertexBuffer.addData(	&uv[i],
									sizeof(Vector2f));
		}

	}

	void Billboard::ensureUpdatedVertexArray() const {

		if(m_vertexArray.needsUpdate()){
			m_vertexArray.bind();
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			m_vertexBuffer.bind();

			// Positions
			glVertexAttribPointer(	0,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector2f),
									(void*)0);
			// UVs
			glVertexAttribPointer(	1,
									2,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector2f),
									(void*)sizeof(Vector3f));

			m_vertexArray.unbind();

			m_vertexArray.setUpdated();
		}

	}

} /* namespace burn */
