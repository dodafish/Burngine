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

	void Blur::apply(Texture& texture) {

		ensureContext();
		glBlendFunc(GL_ONE, GL_ZERO);    // Overwrite

		if(!m_texture.isLoaded())
			m_texture.loadFromData(	texture.getDimensions(),
									Texture::RG16F,
									Texture::DATA_RG,
									0);

		// Framebuffer for first pass towards private texture
		m_framebuffer.create(texture.getDimensions(), false, m_texture);

		m_framebuffer.clear();
		if(m_framebuffer.prepare()){

			const Shader& shader = BurnShaders::getShader(BurnShaders::BLUR);
			shader.resetTextureUnitCounter();
			shader.setUniform("gIsSecondPass", false);
			shader.setUniform("gBlurWidth", 1.f / texture.getDimensions().x);
			shader.setUniform("gProjectionMatrix", m_framebuffer.getOrtho());
			shader.bindTexture("gSampler", texture);

			Sprite s;
			s.setDimensions(Vector2f(texture.getDimensions()));
			s.render(shader);

		}

		// Create framebuffer for second pass
		m_framebuffer.create(texture.getDimensions(), false, texture);

		m_framebuffer.clear();
		if(m_framebuffer.prepare()){

			const Shader& shader = BurnShaders::getShader(BurnShaders::BLUR);
			shader.resetTextureUnitCounter();
			shader.setUniform("gIsSecondPass", true);
			shader.setUniform("gBlurWidth", 1.f / texture.getDimensions().y);
			shader.setUniform("gProjectionMatrix", m_framebuffer.getOrtho());
			shader.bindTexture("gSampler", m_texture);

			Sprite s;
			s.setDimensions(Vector2f(texture.getDimensions()));
			s.render(shader);

		}

	}

} /* namespace burn */
