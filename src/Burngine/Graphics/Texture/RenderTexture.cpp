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

#include <Burngine/Graphics/Texture/RenderTexture.hpp>
#include <Burngine/System/Error.hpp>

namespace burn {

	bool RenderTexture::create(const Vector2ui& dimensions) {

		m_dimensions = dimensions;

		ensureContext();

		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		// Just create an empty one
		m_texture.loadFromData(dimensions, 24, NULL);
		glBindTexture(GL_TEXTURE_2D, m_texture.getId());

		glGenRenderbuffers(1, &m_depthbuffer);
		glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
		glRenderbufferStorage( GL_RENDERBUFFER,
		GL_DEPTH_COMPONENT,
								m_dimensions.x, m_dimensions.y);
		glFramebufferRenderbuffer( GL_FRAMEBUFFER,
		GL_DEPTH_ATTACHMENT,
									GL_RENDERBUFFER, m_depthbuffer);

		glFramebufferTexture( GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0,
								m_texture.getId(), 0);

		GLenum DrawBuffers[1] = {
		GL_COLOR_ATTACHMENT0 };
		glDrawBuffers(1, DrawBuffers);

		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			burnWarn("Failed to create RenderTexture.");
			return false;
		}

		// Unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		return true;
	}

	void RenderTexture::clear() {
		ensureContext();
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const Texture& RenderTexture::getTexture() const{
		return m_texture;
	}

	bool RenderTexture::prepare() const {
		ensureContext();
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glViewport(0, 0, m_dimensions.x, m_dimensions.y);
		return true;
	}

	Matrix4f RenderTexture::getOrtho() const {
		return glm::ortho(	0.f,
							static_cast<float>(m_dimensions.x),
							static_cast<float>(m_dimensions.y),
							0.f);
	}

} /* namespace burn */
