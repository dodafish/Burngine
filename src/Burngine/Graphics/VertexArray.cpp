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

#include <Burngine/Graphics/VertexArray.hpp>
#include <Burngine/System/Thread.hpp>

namespace burn {

	VertexArray::ThreadId::ThreadId() :
	id(0),
	isUpdated(false) {

	}

	VertexArray::VertexArray() :
	m_count(new Uint32(1)) {

	}

	VertexArray::VertexArray(const VertexArray& other) :
	GlEntity(other),
	m_threadIds(other.m_threadIds),
	m_count(other.m_count) {

		++(*m_count);

	}

	VertexArray& VertexArray::operator=(const VertexArray& other) {

		if(this == &other)
			return *this;

		if(--(*m_count) == 0){
			ensureContext();
			for(std::map<void*, ThreadId>::iterator it = m_threadIds.begin(); it != m_threadIds.end(); ++it){
				glDeleteVertexArrays(1, &(it->second.id));
			}
			delete m_count;
		}

		m_threadIds = other.m_threadIds;
		m_count = other.m_count;

		++(*m_count);

		return *this;
	}

	VertexArray::~VertexArray() {
		if(--(*m_count) == 0){
			ensureContext();
			for(std::map<void*, ThreadId>::iterator it = m_threadIds.begin(); it != m_threadIds.end(); ++it){
				glDeleteVertexArrays(1, &(it->second.id));
			}
			delete m_count;
		}
	}

	void VertexArray::bind() const {
		ensureContext();
		glBindVertexArray(getThreadId()->id);
	}

	void VertexArray::unbind() const {
		ensureContext();
		glBindVertexArray(0);
	}

	bool VertexArray::needsUpdate() const {
		return !getThreadId()->isUpdated;
	}

	void VertexArray::setUpdated() const {
		getThreadId()->isUpdated = true;
	}

	VertexArray::ThreadId* VertexArray::getThreadId() const {

		if(m_threadIds.find(Thread::current()) == m_threadIds.end()){

			ThreadId tid;
			ensureContext();
			glGenVertexArrays(1, &tid.id);
			m_threadIds[Thread::current()] = tid;

		}

		return &m_threadIds[Thread::current()];
	}

} /* namespace burn */
