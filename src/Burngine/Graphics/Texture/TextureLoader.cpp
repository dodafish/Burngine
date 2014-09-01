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
#include <bits/functional_hash.h>
#include <fstream>
#include <iostream>

namespace {
	std::hash<std::string> stringToHash;    ///< Converter. Hashes are faster to compare
	burn::Mutex mutex;
}

namespace burn {
	namespace priv {

		std::vector<std::pair<size_t, Texture*>> TextureLoader::m_textures;
		size_t TextureLoader::m_count = 0;

		bool TextureLoader::loadFromFile(	const std::string& file,
											Texture& target) {

			// Check if texture is already loaded
			{
				Lock lock(mutex);
				for(size_t i = 0; i != m_textures.size(); ++i)
					if(m_textures[i].first == (stringToHash(file))){
						target = *(m_textures[i].second);
						return true;
					}
			}

			/*
			 * Not loaded yet.
			 */

			// Check the string
			// There have to be at least 5 characters. E.g.: "a.bmp"
			// -> ".bmp" is illegal
			if(file.size() < 5){
				burnWarn("Cannot load texture '" + file
				+ "'! Filepath is too short.");
				return false;
			}

			// Get filetype
			FileType fileType = checkFileType(file);

			bool result = false;
			if(fileType == BMP)
				result = loadBmp(file, target);
			else{
				burnWarn("Cannot load texture '" + file
				+ "'! File type is not supported.");
				return false;
			}

			// Success?
			if(!result){
				burnWarn("Failed loading texture'" + file + "'!");
				return false;
			}

			// Store the loaded texture
			Lock lock(mutex);
			m_textures.push_back(std::pair<size_t, Texture*>(stringToHash(file),
															new Texture(target)));
			++m_count;

			return true;
		}

		void TextureLoader::cleanup() {

			std::cout << "TextureLoader cleanup start.\n";

			Lock lock(mutex);
			for(size_t i = 0; i != m_textures.size(); ++i)
				delete m_textures[i].second;
			m_textures.clear();
			m_count = 0;

			std::cout << "TextureLoader cleanup done.\n";

		}

		const size_t& TextureLoader::getTextureCount(){
			return m_count;
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

		bool TextureLoader::loadBmp(const std::string& filename,
									Texture& texture) {

			// Try to open the file
			std::fstream file(filename, std::ios::in | std::ios::binary);
			if(!file.is_open()){
				burnWarn("Cannot load bitmap '" + filename
				+ "'! Unable to open file.");
				return false;
			}

			// Data read from the header of the BMP file
			Uint8 header[54];    // BMP header
			Uint32 dataPos;    // Position in the file where the actual data begins
			Uint32 width, height;    // Texture width and height
			Uint32 imageSize;    // = width*height*3
			Uint8 bpp;    // bits per pixel (rgb or rgba?)

			// Read the header
			file.seekg(0, std::ios::beg);
			if(!file.read(reinterpret_cast<char*>(header), 54)){
				burnWarn("Cannot load bitmap '" + filename
				+ "'! Unable to load header.");
				return false;
			}

			// Check header - header always starts with 'BM'
			if(header[0] != 'B' || header[1] != 'M'){
				burnWarn("Cannot load bitmap '" + filename
				+ "'! Corrupt header format.");
				return false;
			}

			// Read information from the header
			dataPos = *(int*)&(header[0x0A]);
			imageSize = *(int*)&(header[0x22]);
			width = *(int*)&(header[0x12]);
			height = *(int*)&(header[0x16]);
			bpp = *(int*)&(header[0x1C]);

			// Check information
			if(width == 0 || height == 0 || (bpp != 24 && bpp != 32)){
				burnWarn("Cannot load bitmap '" + filename
				+ "'! Corrupt header information.");
				return false;
			}

			// Some BMP files are misformatted. Try recover missing information
			if(imageSize == 0)
				imageSize = width * height * 3;    // All pixels, 3 bytes each
			if(dataPos == 0)
				dataPos = 54;    // Right after the header

			std::cout << "Texture info: Format=" << width << "x" << height
			<< "x" << (Uint32)bpp << ", Size=" << imageSize << " bytes.\n";

			// Now we can read the actual image

			// Create buffer. RGB data
			Uint8* data = new Uint8[imageSize];

			// Read the image data
			file.seekg(dataPos, std::ios::beg);
			if(!file.read(reinterpret_cast<char*>(data), imageSize)){
				burnWarn("Cannot load bitmap '" + filename
				+ "'! Failed to load image data.");
				delete[] data;
				return false;
			}

			// Everything is loaded into memory now
			file.close();

			if(bpp == 24){
				// Data has BGR pixels
				// Use data directly

				bgrToRgb(data, width * height * 3);
				texture.loadFromData(Vector2ui(width, height), 24, data);

			}else{
				// Data has BGRA pixels
				// Extract the BGR triplets

				Uint8* pixels = new Uint8[width * height * 3];
				Uint8* cur = &data[0];    // Data cursor
				for(Uint32 i = 0; i < width * height * 3; i += 3){

					// Get RGB
					pixels[i + 2] = *cur++;
					pixels[i + 1] = *cur++;
					pixels[i] = *cur++;

					// Skip alpha
					++cur;

				}

				texture.loadFromData(Vector2ui(width, height), 24, pixels);

				delete[] pixels;
			}

			// Free memory
			delete[] data;

			return true;
		}

		void TextureLoader::bgrToRgb(	Uint8* data,
										const Uint32& size) {

			for(Uint32 i = 0; i < size; i += 3){
				Uint8 r = data[i + 2];
				Uint8 g = data[i + 1];
				Uint8 b = data[i];
				data[i] = r;
				data[i + 1] = g;
				data[i + 2] = b;
			}

		}

	} /* namespace priv */
} /* namespace burn */
