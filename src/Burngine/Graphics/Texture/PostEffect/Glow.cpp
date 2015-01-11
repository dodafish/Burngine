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
#include <Burngine/Graphics/Shader/Shader.hpp>
#include <Burngine/System/Error.hpp>

namespace burn {

	Glow::Glow() :
	m_resolution(0) {

		m_blur.setBlurScale(2.f);

	}

	void Glow::passInput(const Texture2D& texture) {

		if(!m_framebufferExtract.isCreated())
			return;

		m_framebufferExtract.clear();
		if(m_framebufferExtract.prepare()){

			// Extract glowing pixels

			// Create transform to propely scale sprite
			Transformable2D t;
			t.setScale(Vector2f(m_resolution));

			// Setup shader
			const Shader& shader = BurnShaders::getShader(BurnShaders::GLOW);
			shader.resetTextureUnitCounter();
			shader.setUniform("gProjectionMatrix", m_framebufferExtract.getOrtho());
			shader.setUniform("gModelMatrix", t.getModelMatrix());
			shader.bindTexture("gSampler", texture);

			// Execute shader
			Sprite s;
			s.render(shader);

			// Blur extracted pixels
			m_blur.passInput(m_texture);

		}

	}

	void Glow::getOutput(const RenderTarget& output) {

		if(!m_texture.isLoaded())
			return;

		ensureContext();
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		m_blur.getOutput(output);

	}

	bool Glow::create(	const Vector2ui& resolution,
						const GLint& internalFormat,
						const GLenum& dataFormat,
						const GLenum& dataType) {

		// Recreate only if necessary
		if(m_resolution == resolution)
			return true;

		// Recreate framebuffer and texture
		if(!m_texture.loadFromData(resolution, internalFormat, dataFormat, dataType, 0)){
			burnWarn("Failed to create helper texture.");
			return false;
		}
		if(!m_framebufferExtract.create(resolution, false, m_texture)){
			burnWarn("Failed to create helper framebuffer.");
			return false;
		}

		// Setup blur effect
		m_blur.create(resolution, internalFormat, dataFormat, dataType);

		// Save new resolution
		m_resolution = resolution;

		return true;

	}

	bool Glow::create(const Texture2D& texture) {

		if(!texture.isLoaded())
			return false;

		// Recreate framebuffer and texture
		if(!m_texture.loadFromData(texture.getDimensions(), texture.getInternalFormat(),
									texture.getDataFormat(),
									texture.getDataType(), 0)){
			burnWarn("Failed to create helper texture.");
			return false;
		}

		if(!m_framebufferExtract.create(texture.getDimensions(), false, m_texture)){
			burnWarn("Failed to create helper framebuffer.");
			return false;
		}

		// Setup blur effect
		m_blur.create(texture);

		// Save new resolution
		m_resolution = texture.getDimensions();

		return true;
	}

} /* namespace burn */
