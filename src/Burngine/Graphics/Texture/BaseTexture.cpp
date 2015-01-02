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

#include <Burngine/Graphics/Texture/BaseTexture.hpp>

namespace burn {

	BaseTexture::BaseTexture() :
	m_id(0),
	m_dimensions(0),
	m_dataFormat(GL_RGB),
	m_dataType(GL_UNSIGNED_BYTE),
	m_count(new Uint32(1)) {

	}

	BaseTexture::BaseTexture(const BaseTexture& other) :
	GlEntity(other),
	m_id(other.m_id),
	m_dimensions(other.m_dimensions),
	m_dataFormat(other.m_dataFormat),
	m_dataType(other.m_dataType),
	m_count(other.m_count) {

		++(*m_count);

	}

	BaseTexture& BaseTexture::operator=(const BaseTexture& other) {

		if(this == &other)
			return *this;

		--(*m_count);
		if((*m_count) == 0){
			cleanup();
			delete m_count;
		}

		m_id = other.m_id;
		m_dimensions = other.m_dimensions;
		m_dataFormat = other.m_dataFormat;
		m_dataType = other.m_dataType;
		m_count = other.m_count;

		++(*m_count);

		return *this;
	}

	BaseTexture::~BaseTexture() {

		--(*m_count);
		if((*m_count) == 0){
			cleanup();
			delete m_count;
		}

	}

	void BaseTexture::cleanup() {

		if(m_id == 0){
			ensureContext();
			glDeleteTextures(	1,
								&m_id);

			m_id = 0;
		}

	}

	const GLuint& BaseTexture::getId() const {
		return m_id;
	}

	bool BaseTexture::isLoaded() const {
		return m_id != 0;
	}

	const Vector2ui& BaseTexture::getDimensions() const {
		return m_dimensions;
	}

	const GLenum& BaseTexture::getDataFormat() const {
		return m_dataFormat;
	}

	const GLenum& BaseTexture::getDataType() const {
		return m_dataType;
	}

} /* namespace burn */
