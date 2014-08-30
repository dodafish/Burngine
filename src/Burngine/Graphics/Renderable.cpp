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

#include <Burngine/Graphics/Renderable.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/System/Thread.hpp>

namespace burn {

	Renderable::~Renderable() {
		ensureContext();

		for(std::map<void*, GLuint>::iterator it = m_vaoMap.begin(); it != m_vaoMap.end(); ++it)
			glDeleteVertexArrays(1, &(it->second));
	}

	void Renderable::bindVao() const {
		ensureContext();
		glBindVertexArray(getVao());
	}

	void Renderable::unbindVao() const {
		ensureContext();
		glBindVertexArray(0);
	}

	const GLuint& Renderable::getVao() const {

		if(m_vaoMap.find(Thread::current()) == m_vaoMap.end()){
			// New thread/context = new VAO:
			ensureContext();

			GLuint vao = 0;
			glGenVertexArrays(1, &vao);

			m_vaoMap[Thread::current()] = vao;

			updateVao();
		}

		return m_vaoMap[Thread::current()];
	}

} /* namespace burn */
