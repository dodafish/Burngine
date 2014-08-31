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

#include <Burngine/Graphics/Texture/TextureLoader.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Window/GlContext.hpp>
#include <Burngine/System/Mutex.hpp>
#include <Burngine/System/Lock.hpp>
#include <hash_set>
#include <fstream>

namespace {
	std::hash<std::string> stringToHash;    ///< Converter. Hashes are faster to compare
	burn::Mutex mutex;
}

namespace burn {
	namespace priv {

		std::map<size_t, GLuint> TextureLoader::m_textures;

		GLuint TextureLoader::loadFromFile(const std::string& file) {

			// Check if texture is already loaded
			if(m_textures.find(stringToHash(file)) != m_textures.end())
				return m_textures[stringToHash(file)];

			/*
			 * Not loaded yet.
			 */

			// Check the string
			// There have to be at least 5 characters. E.g.: "a.bmp"
			// -> ".bmp" is illegal
			if(file.size() < 5){
				burnWarn("Cannot load texture '" + file + "'! Filepath is too short.");
				return 0;
			}

			// Get filetype
			FileType fileType = checkFileType(file);
			if(fileType == UNKNOWN){
				burnWarn("Cannot load texture '" + file + "'! File type is not supported.");
				return 0;
			}

			GLuint texture = 0;
			if(fileType == BMP)
				texture = loadBmp(file);

			// Success?
			if(texture == 0){
				burnWarn("Failed loading texture'" + file + "'!");
				return 0;
			}

			// Store the loaded texture
			Lock lock(mutex);
			m_textures[stringToHash(file)] = texture;

			return texture;
		}

		TextureLoader::FileType TextureLoader::checkFileType(const std::string& file) {

			std::string endRaw = file.substr(file.size() - 3, 3);
			std::string end = "";

			// Convert all chars to upper case
			for(int i = 0; i != 3; ++i)
				end += std::toupper(endRaw[i]);

			if(end == "BMP")
				return BMP;

			return UNKNOWN;
		}

		GLuint TextureLoader::loadBmp(const std::string& filename) {

			// Try to open the file
			std::basic_fstream<Uint8> file(filename, std::ios::in | std::ios::binary);
			if(!file.is_open()){
				burnWarn("Cannot load bitmap '" + file + "'! Unable to open file.");
				return 0;
			}

			// Data read from the header of the BMP file
			Uint8 header[54];    // BMP header
			Uint32 dataPos;    // Position in the file where the actual data begins
			Uint32 width, height;    // Texture width and height
			Uint32 imageSize;    // = width*height*3

			// Read the header
			if(!file.read(&header[0], 54)){
				burnWarn("Cannot load bitmap '" + file + "'! Unable to load header.");
				return 0;
			}

			// Check header - header always starts with 'BM'
			if(header[0] != 'B' || header[1] != 'M'){
				burnWarn("Cannot load bitmap '" + file + "'! Corrupt header format.");
				return 0;
			}

			// Read information from the header
			dataPos = *(int*)&(header[0x0A]);
			imageSize = *(int*)&(header[0x22]);
			width = *(int*)&(header[0x12]);
			height = *(int*)&(header[0x16]);

			// Check information
			if(width == 0 || height == 0){
				burnWarn("Cannot load bitmap '" + file + "'! Corrupt header information.");
				return 0;
			}

			// Some BMP files are misformatted. Try recover missing information
			if(imageSize == 0)
				imageSize = width * height * 3;    // All pixels, 3 bytes each
			if(dataPos == 0)
				dataPos = 54;    // Right after the header

			// Now we can read the actual image

			// Create buffer. RGB data
			Uint8* data = new Uint8[imageSize];

			// Read the image data
			if(!file.read(data, imageSize)){
				burnWarn("Cannot load bitmap '" + file + "'! Failed to load image data.");
				delete[] data;
				return 0;
			}

			// Everything is loaded into memory now
			file.close();

			// Create texture in OpenGL
			priv::GlContext::ensureContext();
			GLuint texture = 0;
			glGenTextures(1, &texture);

			// Bind and pass the image
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
			glBindTexture(GL_TEXTURE_2D, 0);

			// Free memory
			delete[] data;

			return texture;
		}

	} /* namespace priv */
} /* namespace burn */
