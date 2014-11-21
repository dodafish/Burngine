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

#include <Burngine/Graphics/Texture/PostEffect/Glow.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Gui/Sprite.hpp>

namespace burn {

	void Glow::apply(	Texture& texture,
						Framebuffer* attachedFramebuffer) {

		ensureContext();
		glBlendFunc(GL_ONE, GL_ZERO);    // Overwrite

		if(!m_texture.isLoaded() || m_texture.getDimensions() != texture.getDimensions()){
			m_texture.loadFromData(texture.getDimensions(), Texture::RGBA, Texture::DATA_RGBA, 0);
			m_framebufferExtract.create(m_texture.getDimensions(), false, m_texture);
		}

		m_framebufferExtract.clear();
		if(m_framebufferExtract.prepare()){

			// Extract pixels

			const Shader& shader = BurnShaders::getShader(BurnShaders::GLOW);
			shader.resetTextureUnitCounter();
			shader.setUniform("gProjectionMatrix", m_framebufferExtract.getOrtho());
			shader.bindTexture("gSampler", texture);

			Sprite s;
			s.setDimensions(Vector2f(texture.getDimensions()));
			s.render(shader);

		}

		// Blur the glow texture
		m_blur.apply(m_texture, &m_framebufferExtract, 2.f);
		m_blur.apply(m_texture, &m_framebufferExtract, 1.f);

		if(attachedFramebuffer == NULL){
			m_framebufferApply.create(texture.getDimensions(), false, texture);
			attachedFramebuffer = &m_framebufferApply;
		}

		//attachedFramebuffer->clear();
		if(attachedFramebuffer->prepare()){

			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			Sprite s;
			s.setTexture(m_texture, true);
			s.render(Matrix4f(1.f), Matrix4f(1.f), attachedFramebuffer->getOrtho());

		}

	}

} /* namespace burn */
