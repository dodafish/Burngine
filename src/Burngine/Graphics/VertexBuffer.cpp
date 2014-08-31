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

#include <Burngine/Graphics/VertexBuffer.hpp>

namespace burn {

	VertexBuffer::VertexBuffer() :
	m_id(0),
	m_count(new Uint32(1)),
	m_isDataUploaded(false) {
		ensureContext();
		glGenBuffers(1, &m_id);
	}

	VertexBuffer::VertexBuffer(const VertexBuffer& other) :
	GlEntity(other),
	m_id(other.m_id),
	m_count(other.m_count),
	m_isDataUploaded(other.m_isDataUploaded) {
		++(*m_count);
	}

	VertexBuffer::~VertexBuffer() {

		--(*m_count);
		if((*m_count) == 0){
			ensureContext();
			glDeleteBuffers(1, &m_id);
			delete m_count;
		}

	}

	VertexBuffer& VertexBuffer::operator=(const VertexBuffer& other) {

		if(this == &other)
			return *this;

		--(*m_count);
		if((*m_count) == 0){
			ensureContext();
			glDeleteBuffers(1, &m_id);
			delete m_count;
		}

		m_id = other.m_id;
		m_count = other.m_count;
		++(*m_count);
		m_isDataUploaded = other.m_isDataUploaded;

		return *this;
	}

	void VertexBuffer::reset() {
		m_data.clear();
		m_isDataUploaded = false;
	}

	void VertexBuffer::addData(	const void* data,
								const unsigned int& size) {
		m_data.insert(m_data.end(), (GLbyte*)data, (GLbyte*)data + size);
		m_isDataUploaded = false;
	}

	void VertexBuffer::copyData(const VertexBuffer& other) {

		// Don't copy itself
		if(this == &other)
			return;

		// Last instance? Just reuse the resources
		if((*m_count) == 1){
			m_data = other.getData();
		}
		else{ // Not last instance: Create new resources
			//Decrease the old counter
			--(*m_count);

			// Reset
			m_id = 0;
			m_count = new Uint32(1);
			m_data = other.getData();

			// Generate new ID
			ensureContext();
			glGenBuffers(1, &m_id);

		}

		// Make sure to upload on next use
		m_isDataUploaded = false;
	}

	const std::vector<GLbyte>& VertexBuffer::getData() const {
		return m_data;
	}

	void VertexBuffer::bind() const {

		ensureContext();

		glBindBuffer(GL_ARRAY_BUFFER, m_id);

		if(!m_isDataUploaded){
			glBufferData(GL_ARRAY_BUFFER, m_data.size(), &m_data[0], GL_STATIC_DRAW);
			m_isDataUploaded = true;
		}

	}

	void VertexBuffer::unbind() const {
		ensureContext();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

} /* namespace burn */
