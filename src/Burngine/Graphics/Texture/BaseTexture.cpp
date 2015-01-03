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
	m_internalFormat(GL_RGB),
	m_dataFormat(GL_RGB),
	m_dataType(GL_UNSIGNED_BYTE),
	m_count(new Uint32(1)),
	m_samplerId(0),
	m_magFilter(MAG_BILINEAR),
	m_minFilter(MIN_BILINEAR) {

		ensureContext();

	}

	BaseTexture::BaseTexture(const BaseTexture& other) :
	GlEntity(other),
	m_id(other.m_id),
	m_dimensions(other.m_dimensions),
	m_internalFormat(other.m_internalFormat),
	m_dataFormat(other.m_dataFormat),
	m_dataType(other.m_dataType),
	m_count(other.m_count),
	m_samplerId(other.m_samplerId),
	m_magFilter(other.m_magFilter),
	m_minFilter(other.m_minFilter) {

		++(*m_count);

		ensureContext();

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
		m_internalFormat = other.m_internalFormat;
		m_dataFormat = other.m_dataFormat;
		m_dataType = other.m_dataType;
		m_count = other.m_count;
		m_samplerId = other.m_samplerId;
		m_magFilter = other.m_magFilter;
		m_minFilter = other.m_minFilter;

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
			glDeleteSamplers(	1,
								&m_samplerId);
			m_id = 0;
			m_samplerId = 0;
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

	const GLint& BaseTexture::getInternalFormat() const {
		return m_internalFormat;
	}

	const GLenum& BaseTexture::getDataFormat() const {
		return m_dataFormat;
	}

	const GLenum& BaseTexture::getDataType() const {
		return m_dataType;
	}

	void BaseTexture::setFiltering(	const MagnificationFilter& mag,
									const MinificationFilter& min) {

		m_magFilter = mag;
		m_minFilter = min;

		if(m_samplerId == 0)
			return;

		// Set magnification filter
		if(m_magFilter == MAG_NEAREST)
			glSamplerParameteri(m_samplerId,
								GL_TEXTURE_MAG_FILTER,
								GL_NEAREST);
		else
			//BILINEAR
			glSamplerParameteri(m_samplerId,
								GL_TEXTURE_MAG_FILTER,
								GL_LINEAR);

		// Set minification filter
		if(m_minFilter == MIN_NEAREST)
			glSamplerParameteri(m_samplerId,
								GL_TEXTURE_MIN_FILTER,
								GL_NEAREST);
		else if(m_minFilter == MIN_BILINEAR)
			glSamplerParameteri(m_samplerId,
								GL_TEXTURE_MIN_FILTER,
								GL_LINEAR);
		else if(m_minFilter == MIN_NEAREST_MIPMAP)
			glSamplerParameteri(m_samplerId,
								GL_TEXTURE_MIN_FILTER,
								GL_NEAREST_MIPMAP_NEAREST);
		else if(m_minFilter == MIN_BILINEAR_MIPMAP)
			glSamplerParameteri(m_samplerId,
								GL_TEXTURE_MIN_FILTER,
								GL_LINEAR_MIPMAP_NEAREST);
		else
			//TRILINEAR
			glSamplerParameteri(m_samplerId,
								GL_TEXTURE_MIN_FILTER,
								GL_LINEAR_MIPMAP_LINEAR);

	}

	const BaseTexture::MagnificationFilter& BaseTexture::getMagnificationFilter() const {
		return m_magFilter;
	}

	const BaseTexture::MinificationFilter& BaseTexture::getMinificationFilter() const {
		return m_minFilter;
	}

	void BaseTexture::setSamplerParameter(	const GLenum& parameter,
											const GLenum& value) {

		if(m_samplerId == 0)
			return;

		ensureContext();

		glSamplerParameteri(m_samplerId,
							parameter,
							value);

	}

} /* namespace burn */
