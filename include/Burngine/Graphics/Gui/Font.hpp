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

#ifndef INCLUDE_BURNGINE_GRAPHICS_GUI_FONT_HPP_
#define INCLUDE_BURNGINE_GRAPHICS_GUI_FONT_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <string>

namespace burn {

	/**
	 * @brief Generates textures to render 2D text/characters.
	 */
	class BURNGINE_API_EXPORT Font {
	public:

		Font();

		/**
		 * @brief Loads a font from file that can be used for rendering
		 * 2D text.
		 *
		 * @param file Font file to load from
		 *
		 * @return True on success
		 */
		bool loadFromFile(const std::string& file);

		/**
		 * @brief Check if the font is already loaded.
		 *
		 * @return True if font is loaded. False otherwise.
		 */
		bool isLoaded() const;

		/**
		 * @brief Generate/Load a texture that can be used for rendering the
		 * given character.
		 *
		 * @param charcode Character's unique code
		 * @param fontSize Required font size
		 *
		 * @return Character texture
		 */
		const Texture& getTexture(	const Uint32& charcode,
									const Uint32& fontSize) const;

	private:
		static void* m_ftLibrary;    ///< The freetype library

	private:
		void* m_ftFace;    ///< Loaded font face
	};

} /* namespace burn */

#endif /* INCLUDE_BURNGINE_GRAPHICS_GUI_FONT_HPP_ */
