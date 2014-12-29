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

namespace burn {

	Label::Label() :
	m_fontSize(12) {

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

	void Label::setText(const std::string& text) {
		m_text = text;
	}

	const std::string& Label::getText() const {
		return m_text;
	}

	void Label::render(	const Matrix4f& model,
						const Matrix4f& view,
						const Matrix4f& projection) const {

		// Is a font loaded?
		if(!m_font.isLoaded())
			return;

		Sprite s;

		// Render every character with the help of Sprite
		for(size_t i = 0; i < m_text.size(); ++i){

			const Texture& texture = m_font.getTexture(	(Uint32)(m_text[i]),
														m_fontSize);
			if(!texture.isLoaded()){
				// Could not generate a texture
				continue;
			}

			// Set texture and make sprite fit to it
			s.setTexture(	texture,
							true);

			// Apply transformation
			s.setPosition(getPosition());
			s.setRotation(getRotation());
			s.setScale(getScale());

			// Finally, render the character
			s.render(	model,
						view,
						projection);

		}

	}

	void Label::render(const Shader& shader) const {

		// Is a font loaded?
		if(!m_font.isLoaded())
			return;

		Sprite s;

		// Render every character with the help of Sprite
		for(size_t i = 0; i < m_text.size(); ++i){

			const Texture& texture = m_font.getTexture(	(Uint32)(m_text[i]),
														m_fontSize);
			if(!texture.isLoaded()){
				// Could not generate a texture
				continue;
			}

			// Set texture and make sprite fit to it
			s.setTexture(	texture,
							true);

			// Apply transformation
			s.setPosition(getPosition());
			s.setRotation(getRotation());
			s.setScale(getScale());

			// Finally, render the character
			s.render(shader);

		}

	}

} /* namespace burn */
