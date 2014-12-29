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

#include <ft2build.h>
#include FT_FREETYPE_H

namespace burn {

	void* Font::m_ftLibrary = NULL;

	Font::Font() :
	m_ftFace(NULL) {

	}

	bool Font::loadFromFile(const std::string& file) {

		// Our void* pointer is the ft library
		FT_Library library = (FT_Library)m_ftLibrary;

		// Init freetype library if not done yet
		if(library == NULL){
			FT_Error error = FT_Init_FreeType(&library);
			if(error)
				burnErr("Cannot initialize freetype library.");
		}

		// Also convert the void* font face pointer
		FT_Face face = (FT_Face)m_ftFace;

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

		return true;
	}

	bool Font::isLoaded() const {
		return (m_ftFace != NULL);
	}

} /* namespace burn */
