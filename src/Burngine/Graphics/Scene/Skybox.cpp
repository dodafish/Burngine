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

#include <Burngine/Graphics/Scene/Skybox.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>
#include <Burngine/Graphics/Scene/Transformable3D.hpp>

namespace burn {

	Skybox::Skybox() {
		updateVertexData();
	}

	void Skybox::render(const Vector3f& cameraPosition,
						const Matrix4f& view,
						const Matrix4f& projection) const {

		if(!m_cubeMap.isLoaded())
			return;

		// We need an OpenGL context
		ensureContext();
		// Our data has to be uploaded
		ensureUpdatedVertexArray();

		Transformable3D t;
		t.setPosition(cameraPosition);

		const Shader& shader = BurnShaders::getShader(BurnShaders::SKYBOX);
		shader.resetTextureUnitCounter();
		shader.bindTexture("gCubeMap", m_cubeMap);
		shader.setUniform("gModelMatrix", t.getLocalModelMatrix());
		shader.setUniform("gViewMatrix", view);
		shader.setUniform("gProjectionMatrix", projection);

		// render
		m_vertexArray.bind();
		glDrawArrays( GL_TRIANGLES,
						0,
						36);
		m_vertexArray.unbind();

	}

	void Skybox::setCubeMap(const CubeMap& cubeMap) {
		m_cubeMap = cubeMap;
	}

	const CubeMap& Skybox::getCubeMap() const {
		return m_cubeMap;
	}

	void Skybox::updateVertexData() {

		// Cube mesh:
		static Vector3f vertices[] = {
		Vector3f(+1.f, -1.f, +1.f),    // positive x side
		Vector3f(+1.f, -1.f, -1.f),
		Vector3f(+1.f, +1.f, +1.f),
		Vector3f(+1.f, -1.f, -1.f),
		Vector3f(+1.f, +1.f, +1.f),
		Vector3f(+1.f, +1.f, -1.f),
		Vector3f(-1.f, -1.f, +1.f),    // negative x side
		Vector3f(-1.f, -1.f, -1.f),
		Vector3f(-1.f, +1.f, +1.f),
		Vector3f(-1.f, -1.f, -1.f),
		Vector3f(-1.f, +1.f, +1.f),
		Vector3f(-1.f, +1.f, -1.f),
		Vector3f(-1.f, +1.f, +1.f),    // positive y side
		Vector3f(-1.f, +1.f, -1.f),
		Vector3f(+1.f, +1.f, -1.f),
		Vector3f(+1.f, +1.f, -1.f),
		Vector3f(-1.f, +1.f, +1.f),
		Vector3f(+1.f, +1.f, +1.f),
		Vector3f(-1.f, -1.f, +1.f),    // negative y side
		Vector3f(-1.f, -1.f, -1.f),
		Vector3f(+1.f, -1.f, -1.f),
		Vector3f(+1.f, -1.f, -1.f),
		Vector3f(-1.f, -1.f, +1.f),
		Vector3f(+1.f, -1.f, +1.f),
		Vector3f(-1.f, +1.f, +1.f),    // positive z side
		Vector3f(-1.f, -1.f, +1.f),
		Vector3f(+1.f, -1.f, +1.f),
		Vector3f(-1.f, +1.f, +1.f),
		Vector3f(+1.f, -1.f, +1.f),
		Vector3f(+1.f, +1.f, +1.f),
		Vector3f(-1.f, +1.f, -1.f),    // negative z side
		Vector3f(-1.f, -1.f, -1.f),
		Vector3f(+1.f, -1.f, -1.f),
		Vector3f(-1.f, +1.f, -1.f),
		Vector3f(+1.f, -1.f, -1.f),
		Vector3f(+1.f, +1.f, -1.f)
		};

		m_vertexBuffer.reset();
		for(int i = 0; i != 36; ++i)
			m_vertexBuffer.addData(&(vertices[i]), sizeof(Vector3f));

	}

	void Skybox::ensureUpdatedVertexArray() const {

		if(m_vertexArray.needsUpdate()){

			m_vertexArray.bind();
			glEnableVertexAttribArray(0);
			m_vertexBuffer.bind();
			glVertexAttribPointer(0,
									3,
									GL_FLOAT,
									GL_FALSE,
									0,
									(void*)0);

			m_vertexArray.unbind();

			m_vertexArray.setUpdated();
		}

	}

} /* namespace burn */
