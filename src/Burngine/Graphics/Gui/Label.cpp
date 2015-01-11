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

#include <Burngine/Graphics/Gui/Label.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>
#include <Burngine/Graphics/Gui/Sprite.hpp>

namespace burn {

	Label::Label() :
	m_fontSize(12),
	m_color(1.f) {

	}

	void Label::setFont(const Font& font) {
		m_font = font;
	}

	const Font& Label::getFont() const {
		return m_font;
	}

	void Label::setFontSize(const Uint32& fontSize) {
		m_fontSize = fontSize;
	}

	const Uint32& Label::getFontSize() const {
		return m_fontSize;
	}

	void Label::setText(const String& text) {
		m_text = text;
	}

	const String& Label::getText() const {
		return m_text;
	}

	void Label::render(	const Matrix4f& view,
						const Matrix4f& projection) const {

		// Is a font loaded?
		if(!m_font.isLoaded())
			return;

		// Per character offset (move towards right in row)
		Vector2ui offset;

		// Transform per character
		Transformable2D subTransform;

		// Render every character
		for(size_t i = 0; i < m_text.getSize(); ++i){

			// Get the glyph texture of the character
			const Font::Character& c = m_font.getCharacter((m_text[i]),
															m_fontSize);

			// Check if it could be loaded successfully
			if(!c.texture.isLoaded()){
				// Could not generate a texture
				continue;
			}

			// Use a sprite as helper
			static Sprite s;

			// Set individual y-offset
			offset.y = getPosition().y - c.texture.getDimensions().y + c.vertOff;

			// Apply transformation
			subTransform.setPosition(getPosition() + Vector2f(offset));
			subTransform.setRotation(getRotation());
			subTransform.setScale(getScale() * Vector2f(c.texture.getDimensions()));

			// Setup shader
			const Shader& shader = BurnShaders::getShader(BurnShaders::FONT2D);
			shader.resetTextureUnitCounter();
			shader.setUniform("gModelMatrix", subTransform.getModelMatrix());
			shader.setUniform("gViewMatrix", view);
			shader.setUniform("gProjectionMatrix", projection);
			shader.setUniform("gColor", m_color);
			shader.bindTexture("gTextureSampler", c.texture);

			// Tell OpenGL to render the character
			s.render(shader);

			// Move pen
			offset += c.advance;
		}

	}

	void Label::setColor(const Vector4f& color) {
		m_color = color;
	}

	const Vector4f& Label::getColor() const {
		return m_color;
	}

} /* namespace burn */
