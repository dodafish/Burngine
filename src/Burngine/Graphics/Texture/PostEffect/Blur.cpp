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
#include <Burngine/Graphics/Shader/Shader.hpp>
#include <Burngine/System/Error.hpp>

namespace burn {

	Blur::Blur() :
	m_blurScale(1.f),
	m_resolution(0) {

	}

	void Blur::passInput(const Texture2D& texture) {

		// A framebuffer has to be created (via setResolution())
		if(!m_framebufferFirst.isCreated())
			return;

		ensureContext();
		glBlendFunc(GL_ONE,
		GL_ZERO);    // Overwrite

		// Prepare framebuffer
		m_framebufferFirst.clear();
		if(m_framebufferFirst.prepare()){

			// Setup transform to scale the sprite
			Transformable2D t;
			t.setScale(Vector2f(m_resolution));

			// Setup shader
			const Shader& shader = BurnShaders::getShader(BurnShaders::BLUR);
			shader.resetTextureUnitCounter();
			shader.setUniform("gIsSecondPass", false);
			shader.setUniform("gBlurScale", m_blurScale);
			shader.setUniform("gSamplerDimensions", Vector2f(texture.getDimensions()));
			shader.setUniform("gModelMatrix", t.getModelMatrix());
			shader.setUniform("gProjectionMatrix", m_framebufferFirst.getOrtho());
			shader.bindTexture("gSampler", texture);

			// Render with a sprite
			Sprite s;
			s.render(shader);

		}

	}

	void Blur::getOutput(const RenderTarget& output) {

		// The texture to sample from needs to be created
		if(!m_texture.isLoaded())
			return;

		if(output.prepare()){

			// Setup transform to scale the sprite
			Transformable2D t;
			t.setScale(Vector2f(output.getDimensions()));

			// Setup shader
			const Shader& shader = BurnShaders::getShader(BurnShaders::BLUR);
			shader.resetTextureUnitCounter();
			shader.setUniform("gIsSecondPass", true);
			shader.setUniform("gBlurScale", m_blurScale);
			shader.setUniform("gSamplerDimensions", Vector2f(m_texture.getDimensions()));
			shader.setUniform("gModelMatrix", t.getModelMatrix());
			shader.setUniform("gProjectionMatrix", output.getOrtho());
			shader.bindTexture("gSampler", m_texture);

			// Render with a sprite
			Sprite s;
			s.render(shader);

		}

	}

	bool Blur::create(	const Vector2ui& resolution,
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
		if(!m_framebufferFirst.create(resolution, false, m_texture)){
			burnWarn("Failed to create helper framebuffer.");
			return false;
		}

		m_texture.setSamplerParameter(GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE);
		m_texture.setSamplerParameter(GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE);

		// Save new resolution
		m_resolution = resolution;

		return true;
	}

	bool Blur::create(const Texture2D& texture) {

		if(!texture.isLoaded())
			return false;

		// Recreate framebuffer and texture
		if(!m_texture.loadFromData(	texture.getDimensions(),
									texture.getInternalFormat(),
									texture.getDataFormat(),
									texture.getDataType(),
									0)){
			burnWarn("Failed to create helper texture.");
			return false;
		}
		if(!m_framebufferFirst.create(texture.getDimensions(), false, m_texture)){
			burnWarn("Failed to create helper framebuffer.");
			return false;
		}

		m_texture.setSamplerParameter(GL_TEXTURE_WRAP_S,
		GL_CLAMP_TO_EDGE);
		m_texture.setSamplerParameter(GL_TEXTURE_WRAP_T,
		GL_CLAMP_TO_EDGE);

		// Save new resolution
		m_resolution = texture.getDimensions();

		return true;

	}

	void Blur::setBlurScale(const float& scale) {
		m_blurScale = scale;
	}

} /* namespace burn */
