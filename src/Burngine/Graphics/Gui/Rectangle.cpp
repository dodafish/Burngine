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

#include <Burngine/Graphics/Gui/Rectangle.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>

namespace burn {

	Rectangle::Rectangle() :
	m_color(1.f) {

		// Create data array
		static Vector3f vboData[] = {
		Vector3f(0.f),
		Vector3f(1.f, 0.f, 0.f),
		Vector3f(0.f, 1.f, 0.f),
		Vector3f(1.f, 1.f, 0.f) };

		// Add data to VBO
		m_vertexBuffer.addData(&vboData[0],
								sizeof(vboData));

	}

	void Rectangle::setDimensions(const Vector2f& dimensions) {
		m_dimensions = dimensions;
	}

	const Vector2f& Rectangle::getDimensions() const {
		return m_dimensions;
	}

	void Rectangle::setColor(const Vector4f& color) {
		m_color = color;
	}

	const Vector4f& Rectangle::getColor() const {
		return m_color;
	}

	void Rectangle::render(	const Matrix4f& view,
							const Matrix4f& projection) const {

		// We need an OpenGL context
		ensureContext();
		// Our data has to be uploaded
		ensureUpdatedVertexArray();

		// Create Transformable that takes Rectangle's dimensions into account
		Transformable2D t;
		t.setPosition(getPosition());
		t.setRotation(getRotation());
		t.setScale(getScale() * m_dimensions);

		// Setup shader
		const Shader& shader = BurnShaders::getShader(BurnShaders::COLOR);
		shader.setUniform("gModelMatrix", t.getModelMatrix());
		shader.setUniform("gViewMatrix", view);
		shader.setUniform("gProjectionMatrix", projection);
		shader.setUniform("gColor", m_color);
		shader.activate();

		// Render
		m_vertexArray.bind();
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
		m_vertexArray.unbind();

	}

	void Rectangle::ensureUpdatedVertexArray() const {

		// Update vertex array if necessary
		if(m_vertexArray.needsUpdate()){

			// Setup vertex array
			m_vertexArray.bind();

			// Enable arrays
			glEnableVertexAttribArray(0);

			// Link vertex buffers
			m_vertexBuffer.bind();
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

			// "Save" vertex array
			m_vertexArray.unbind();
			m_vertexArray.setUpdated();
		}

	}

} /* namespace burn */
