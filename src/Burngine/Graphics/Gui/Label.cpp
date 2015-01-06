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

		// We need an OpenGL context
		ensureContext();
		// Our data has to be uploaded
		ensureUpdatedVertexArray();

		// Per character offset (move towards right in row)
		Vector2ui offset;

		// Transform per character
		Transformable2D subTransform;

		Sprite s;

		// Render every character
		for(size_t i = 0; i < m_text.size(); ++i){

			// Get the glyph texture of the character
			const Font::Character& c = m_font.getTexture(	(Uint32)(m_text[i]),
															m_fontSize);

			// Check if it could be loaded successfully
			if(!c.texture.isLoaded()){
				// Could not generate a texture
				continue;
			}

			// Resize the sprite properly
			s.setTexture(c.texture, true);

			// Apply transformation
			subTransform.setPosition(getPosition() + Vector2f(offset));
			subTransform.setRotation(getRotation());
			subTransform.setScale(getScale());

			// Finally, render the character
			// Additional model matrix depending on texture area
			//Transformable2D t;
			//t.setScale(Vector2f(c.uvEnd.x,
			//					c.uvEnd.y));

			// Setup shader
			const Shader& shader = BurnShaders::getShader(BurnShaders::FONT2D);
			shader.resetTextureUnitCounter();
			shader.setUniform(	"gModelMatrix",
								subTransform.getModelMatrix() * model);
			shader.setUniform(	"gViewMatrix",
								view);
			shader.setUniform(	"gProjectionMatrix",
								projection);
			shader.setUniform(	"gColor",
								m_color);
			shader.bindTexture(	"gTextureSampler",
								c.texture);

			// Tell OpenGL to render the character
			s.render(shader);

			// Move pen
			offset += c.advance;
		}

	}

	void Label::render(const Shader&) const {

		/*

		 // Is a font loaded?
		 if(!m_font.isLoaded())
		 return;

		 Sprite s;

		 Vector2ui offset;

		 // Render every character with the help of Sprite
		 for(size_t i = 0; i < m_text.size(); ++i){

		 const Font::Character& c = m_font.getTexture(	(Uint32)(m_text[i]),
		 m_fontSize);
		 if(!c.texture.isLoaded()){
		 // Could not generate a texture
		 continue;
		 }

		 // Set texture and make sprite fit to it
		 s.setTexture(	c.texture,
		 true);
		 s.setTextureArea(	Vector2f(0.f),
		 c.uvEnd);

		 // Apply transformation
		 s.setPosition(getPosition() + Vector2f(offset));
		 s.setRotation(getRotation());
		 s.setScale(getScale());

		 // Finally, render the character
		 s.render(shader);

		 // Move pen
		 offset += c.advance;
		 }

		 */

	}

	void Label::ensureUpdatedVertexArray() const {

		if(m_vertexArray.needsUpdate()){

			m_vertexArray.bind();
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			m_vertexBuffer.bind();
			glVertexAttribPointer(	0,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector2f),
									(void*)0);
			glVertexAttribPointer(	1,
									2,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector2f),
									(void*)sizeof(Vector3f));
			m_vertexArray.unbind();

			m_vertexArray.setUpdated();
		}

	}

} /* namespace burn */
