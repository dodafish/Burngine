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

#include <Burngine/Graphics/Gui/Font.hpp>
#include <Burngine/System/Error.hpp>
#include <iostream>

#include <ft2build.h>
#include FT_FREETYPE_H

namespace {

	burn::Uint32 nextP2(const burn::Uint32 n) {
		burn::Uint32 p2 = 1;
		while(p2 < n)
			p2 <<= 1;
		return p2;
	}

}

namespace burn {

	void* Font::m_ftLibrary = NULL;

	Font::Font() :
	m_ftFace(NULL) {

	}

	bool Font::loadFromFile(const std::string& file) {

		// Remove previous loaded characters of old font
		m_characters.clear();

		// Our void* pointer is the ft library
		FT_Library library = (FT_Library)m_ftLibrary;

		// Init freetype library if not done yet
		if(library == NULL){
			FT_Error error = FT_Init_FreeType(&library);
			if(error){
				burnErr("Cannot initialize freetype library.");
				// No return. Program will stop.
			}
			std::cout << "Initialized Freetype.\n";
			m_ftLibrary = library;
		}

		FT_Face face = NULL;

		// Load the standard font face
		FT_Error error = FT_New_Face(	library,
										file.c_str(),
										0,    // 0 = standard/first font face
										&face);

		if(error == FT_Err_Unknown_File_Format){
			burnWarn("Cannot load font '" + file + "'. File format is unknown.");
			return false;
		}else if(error){
			burnWarn("Cannot load font '" + file + "'. File could not be opened, read or is broken.");
			return false;
		}

		m_ftFace = face;

		std::cout << "Loaded font: " << file << "\n";

		return true;
	}

	bool Font::isLoaded() const {
		return (m_ftFace != NULL);
	}

	const Font::Character& Font::getTexture(const Uint32& charcode,
											const Uint32& fontSize) const {

		// Is a font actually loaded?
		if(!isLoaded()){
			// Return empty texture. Won't be rendered.
			static Character emptyChar;
			return emptyChar;
		}

		// Use an already loaded one if possible
		if(m_characters.find(charcode) != m_characters.end()){
			for(size_t i = 0; i < m_characters[charcode].size(); ++i){
				if(m_characters[charcode][i].fontSize == fontSize)
					return m_characters[charcode][i];
			}
		}

		// Turn void* into FT_Face
		FT_Face face = (FT_Face)m_ftFace;

		if(FT_Set_Pixel_Sizes(	face,
								fontSize,
								fontSize) != 0){
			burnErr("Failed setting desired font size.");
			// No return. Program will stop.
		}

		// Get character's index inside the font face
		Uint32 charIndex = FT_Get_Char_Index(	face,
												charcode);

		// Load glyph image into the slot
		if(FT_Load_Glyph(	face,
							charIndex,
							FT_LOAD_DEFAULT) != 0){
			burnErr("Failed selecting glyph into slot.");
			// No return. Program will stop.
		}

		// Generate a bitmap
		FT_Render_Glyph(face->glyph,
						FT_RENDER_MODE_NORMAL);

		// Shortcut
		FT_Bitmap* bitmap = &face->glyph->bitmap;

		// Create power of 2 bitmap
		Uint32 width = nextP2(bitmap->width);
		Uint32 height = nextP2(bitmap->rows);

		Uint8* data = new Uint8[width * height];
		for(Uint32 h = 0; h != height; ++h)
			for(Uint32 w = 0; w != width; ++w)
				if(w < bitmap->width && h < bitmap->rows)
					data[w + h * width] = bitmap->buffer[(bitmap->rows - h - 1) * bitmap->width + w];
				else
					data[w + h * width] = 0;

		// Resulting texture
		Texture2D charTexture;

		// Create texture
		if(!charTexture.loadFromData(	Vector2ui(	width,
													height),
										GL_DEPTH_COMPONENT,
										GL_DEPTH_COMPONENT,
										GL_UNSIGNED_BYTE,
										data)){
			// Free allocated memory
			delete[] data;
			burnErr("Failed loading bitmap.");
			// No return. Program will stop.
		}

		// Set sampling parameters
		charTexture.setSamplerParameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		charTexture.setSamplerParameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		charTexture.setSamplerParameter(GL_DEPTH_TEXTURE_MODE, GL_INTENSITY);
		charTexture.setFiltering(BaseTexture::MAG_BILINEAR, BaseTexture::MIN_BILINEAR);

		// Free allocated memory
		delete[] data;

		// Generated Character
		Character c;
		c.texture = charTexture;
		c.fontSize = fontSize;
		c.advance.x = face->glyph->advance.x >> 6;
		c.advance.y = face->glyph->advance.y >> 6;
		c.uvEnd.x = (float)(bitmap->width) / (float)(width);
		c.uvEnd.y = (float)(bitmap->rows) / (float)(height);

		// Store generated character
		m_characters[charcode].push_back(c);

		std::cout << "Loaded character: " << char(charcode) << "[" << charcode << "]\n";

		return m_characters[charcode].back();
	}

} /* namespace burn */
