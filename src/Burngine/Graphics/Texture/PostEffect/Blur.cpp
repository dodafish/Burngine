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

#include <Burngine/Graphics/Texture/PostEffect/Blur.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Gui/Sprite.hpp>

namespace burn {

	void Blur::apply(	Texture& texture,
						Framebuffer* attachedFramebuffer,
						const float& blurScale) {

		ensureContext();
		glBlendFunc(GL_ONE, GL_ZERO);    // Overwrite

		if(!m_texture.isLoaded() || m_texture.getDimensions() != texture.getDimensions()
		|| m_texture.getPixelFormat() != texture.getPixelFormat()
		|| m_texture.getDataFormat() != texture.getDataFormat()){
			m_texture.loadFromData(	texture.getDimensions(),
									texture.getPixelFormat(),
									texture.getDataFormat(),
									0);
			// Framebuffer for first pass towards private texture
			m_framebufferFirst.create(texture.getDimensions(), false, m_texture);
		}

		m_framebufferFirst.clear();
		if(m_framebufferFirst.prepare()){

			const Shader& shader = BurnShaders::getShader(BurnShaders::BLUR);
			shader.resetTextureUnitCounter();
			shader.setUniform("gIsSecondPass", false);
			shader.setUniform("gBlurScale", blurScale);
			shader.setUniform("gSamplerDimensions", Vector2f(texture.getDimensions()));
			shader.setUniform("gProjectionMatrix", m_framebufferFirst.getOrtho());
			shader.bindTexture("gSampler", texture);

			Sprite s;
			s.setDimensions(Vector2f(texture.getDimensions()));
			s.render(shader);

		}

		// Create framebuffer for second pass
		if(attachedFramebuffer == NULL){
			m_framebufferSecond.create(texture.getDimensions(), false, texture);
			attachedFramebuffer = &m_framebufferSecond;
		}

		attachedFramebuffer->clear();
		if(attachedFramebuffer->prepare()){

			const Shader& shader = BurnShaders::getShader(BurnShaders::BLUR);
			shader.resetTextureUnitCounter();
			shader.setUniform("gIsSecondPass", true);
			shader.setUniform("gBlurScale", blurScale);
			shader.setUniform("gSamplerDimensions", Vector2f(m_texture.getDimensions()));
			shader.setUniform("gProjectionMatrix", attachedFramebuffer->getOrtho());
			shader.bindTexture("gSampler", m_texture);

			Sprite s;
			s.setDimensions(Vector2f(texture.getDimensions()));
			s.render(shader);

		}

	}

} /* namespace burn */
