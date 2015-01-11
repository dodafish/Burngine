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

#include <Burngine/Graphics/Texture/Framebuffer.hpp>
#include <Burngine/System/Error.hpp>

namespace burn {

	Framebuffer::Framebuffer() :
	m_framebuffer(0),
	m_depthbuffer(0) {

	}

	Framebuffer::~Framebuffer() {
		cleanup();
	}

	bool Framebuffer::create(	const Vector2ui& dimensions,
								bool createDepthbuffer,
								Texture2D& colorAttachment) {

		// Check parameters
		if(dimensions.x == 0 || dimensions.y == 0){
			burnWarn("Cannot create framebuffer. Invalid dimenions.");
			return false;
		}

		// First clean up
		cleanup();

		// Save new attributes
		m_dimensions = dimensions;

		ensureContext();

		// Generate framebuffer
		glGenFramebuffers(1, &m_framebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		if(createDepthbuffer){
			// Create the depthbuffer
			glGenRenderbuffers(1, &m_depthbuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, m_depthbuffer);
			glRenderbufferStorage( GL_RENDERBUFFER,
			GL_DEPTH_COMPONENT32F,
									m_dimensions.x, m_dimensions.y);
			glFramebufferRenderbuffer( GL_FRAMEBUFFER,
			GL_DEPTH_ATTACHMENT,
										GL_RENDERBUFFER, m_depthbuffer);
		}

		// Attach the texture to the framebuffer
		attachTexture(colorAttachment, 0);

		// Rebind framebuffer, because attachTexture() automatically
		// unbinds it
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		// Check RenderTexture
		if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE){
			burnWarn("Failed to create framebuffer.");
			return false;
		}

		// Unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindRenderbuffer(GL_RENDERBUFFER, 0);

		return true;
	}

	bool Framebuffer::attachTexture(Texture2D& texture,
									const Uint32& position) {

		// Is the framebuffer created?
		if(!isCreated()){
			burnWarn("Cannot attach texture. Framebuffer is not created.");
			return false;
		}

		// Is the position valid or in use?
		ensureContext();
		if(position >= GL_MAX_COLOR_ATTACHMENTS){
			burnWarn("Cannot attach texture. Attachment position is too high.");
			return false;
		}else{
			// Is the position in use already?
			for(size_t i = 0; i < m_colorAttachments.size(); ++i){
				if(position == m_colorAttachments[i].position){
					burnWarn("Cannot attach texture. Attachment position is already in use.");
					return false;
				}
			}
		}

		// Is the texture created?
		if(!texture.isLoaded()){
			// Just create an empty texture with framebuffer's dimensions
			texture.loadFromData(m_dimensions, GL_RGB, GL_RGB, GL_UNSIGNED_BYTE,
			NULL);
		}else{
			// Check the dimensions
			if(texture.getDimensions() != m_dimensions){
				burnWarn("Cannot attach texture. Texture's dimensions do not fit.");
				return false;
			}
		}

		// Ok, parameters are valid. Attach the texture:

		// Add the new color attachment
		ColorAttachment colorAttachment;
		colorAttachment.texture = texture;
		colorAttachment.position = position;
		m_colorAttachments.push_back(colorAttachment);

		// Bind the framebuffer
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);

		// Attach the texture to the framebuffer
		glFramebufferTexture2D( GL_FRAMEBUFFER,
		GL_COLOR_ATTACHMENT0 + position,
								GL_TEXTURE_2D, texture.getId(), 0);

		// Tell OpenGL the new buffer array
		GLenum drawBuffers[m_colorAttachments.size()];
		for(size_t i = 0; i < m_colorAttachments.size(); ++i){
			drawBuffers[i] = GL_COLOR_ATTACHMENT0 + m_colorAttachments[i].position;
		}

		// Give it to OpenGL
		glDrawBuffers(m_colorAttachments.size(), drawBuffers);

		// Unbind
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		return true;
	}

	void Framebuffer::cleanup() {

		// Remove color attachments from list
		m_colorAttachments.clear();

		// Cleanup framebuffer and optional depthbuffer
		if(m_framebuffer != 0){
			ensureContext();

			glDeleteFramebuffers(1, &m_framebuffer);
			m_framebuffer = 0;

			if(m_depthbuffer != 0){

				glDeleteRenderbuffers(1, &m_depthbuffer);
				m_depthbuffer = 0;

			}
		}

		// Reset attributes
		m_dimensions = Vector2ui(0);

	}

	void Framebuffer::clear() {

		if(!isCreated())
			return;

		ensureContext();
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glClearColor(0.f, 0.f, 0.f, 0.f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	const Vector2ui& Framebuffer::getDimensions() const {
		return m_dimensions;
	}

	bool Framebuffer::prepare() const {

		if(!isCreated())
			return false;

		ensureContext();
		glBindFramebuffer(GL_FRAMEBUFFER, m_framebuffer);
		glViewport(0, 0, m_dimensions.x, m_dimensions.y);
		return true;
	}

	Matrix4f Framebuffer::getOrtho() const {
		return glm::ortho(0.f, static_cast<float>(m_dimensions.x), static_cast<float>(m_dimensions.y), 0.f);
	}

	bool Framebuffer::isCreated() const {
		return (m_framebuffer != 0);
	}

} /* namespace burn */
