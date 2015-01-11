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

#ifndef INCLUDE_BURNGINE_GRAPHICS_GUI_LABEL_HPP_
#define INCLUDE_BURNGINE_GRAPHICS_GUI_LABEL_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Gui/Font.hpp>
#include <Burngine/Graphics/Gui/GuiNode.hpp>
#include <Burngine/System/String.hpp>

namespace burn {

	/**
	 * @brief Displays text with a Font instance
	 */
	class BURNGINE_API_EXPORT Label : public GuiNode {
	public:

		/**
		 * @brief Default initializing constructor
		 */
		Label();

		/**
		 * @brief Set the font used to render text
		 *
		 * @param New font to use
		 */
		void setFont(const Font& font);

		/**
		 * @brief Get the currently used font for text rendering
		 *
		 * @return Currently used font
		 */
		const Font& getFont() const;

		/**
		 * @brief Set the font size in pixels that shall be used for rendering.
		 *
		 * @param fontSize Font size in pixels
		 */
		void setFontSize(const Uint32& fontSize);

		/**
		 * @brief Get the currently used font size in pixels used for rendering.
		 *
		 * @return Used font size in pixels
		 */
		const Uint32& getFontSize() const;

		/**
		 * @brief Set the text that shall be rendered
		 *
		 * @param text Text to render
		 */
		void setText(const String& text);

		/**
		 * @brief Get the text that is currently rendered
		 *
		 * @return Currently set text
		 */
		const String& getText() const;

		void render(const Matrix4f& view,
					const Matrix4f& projection) const;

		void setColor(const Vector4f& color);

		const Vector4f& getColor() const;

	private:
		Font m_font;	///< Font object. Generates textures.
		Uint32 m_fontSize;    ///< Font size in pixels
		String m_text;
		Vector4f m_color;
	};

} /* namespace burn */

#endif /* INCLUDE_BURNGINE_GRAPHICS_GUI_LABEL_HPP_ */
