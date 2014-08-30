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

static const GLfloat vboData[] = {
-1.f,
-1.f,
0.f,
1.f,
-1.f,
0.f,
0.f,
1.f,
0.f };

namespace burn {

	Rectangle::Rectangle() :
	m_vbo(0) {
		ensureContext();
		glGenBuffers(1, &m_vbo);

		// Generate and setup
		bindVao();

		glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vboData), vboData, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		unbindVao();

	}

	Rectangle::~Rectangle() {
		ensureContext();
		glDeleteBuffers(1, &m_vbo);
	}

	void Rectangle::render() const {

		const Shader& shader = BurnShaders::getShader(BurnShaders::COLOR);

		shader.activate();
		bindVao();
		glDrawArrays(GL_TRIANGLES, 0, 3);
		unbindVao();

	}

} /* namespace burn */
