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
			else if(fileType == TGA)
				result = loadTga(file, target);
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
			m_textures.push_back(std::pair<size_t, Texture*>(	stringToHash(file),
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

		const size_t& TextureLoader::getTextureCount() {
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
			else if(end == "TGA")
				return TGA;

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
			Uint32 imageSize;    // = width*height*bpp
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

			std::cout << "Texture info: Type=BMP, Format=" << width << "x"
			<< height << "x" << (Uint32)bpp << ", Size=" << imageSize
			<< " bytes.\n";

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

				texture.loadFromData(	Vector2ui(width, height),
										Texture::RGB,
										Texture::BGR,
										Texture::UNSIGNED_BYTE,
										data);

			}else{
				// Data has BGRA pixels

				texture.loadFromData(	Vector2ui(width, height),
										Texture::RGB,
										Texture::BGRA,
										Texture::UNSIGNED_BYTE,
										data);
			}

			// Free memory
			delete[] data;

			return true;
		}

		bool TextureLoader::loadTga(const std::string& filename,
									Texture& texture) {

			// Try to open the file
			std::fstream file(filename, std::ios::in | std::ios::binary);
			if(!file.is_open()){
				burnWarn("Cannot load TarGA '" + filename
				+ "'! Unable to open file.");
				return false;
			}

			// Data read from the header of the TarGA file
			Uint8 header[18];    // TarGA header
			Uint32 colorMapPos;    // Position where color map begins
			Uint32 width, height;    // Texture width and height
			Uint32 imageSize;    // = width*height*bpp
			//Uint32 colorMapSize;
			Uint8 imageBpp, colorMapBpp;    // bits per pixel (rgb or rgba?)
			bool hasColorMap, isRleCompressed;

			//colorMapSize = 0;

			// Read the header
			file.seekg(0, std::ios::beg);
			if(!file.read(reinterpret_cast<char*>(header), 18)){
				burnWarn("Cannot load TarGA '" + filename
				+ "'! Unable to load header.");
				return false;
			}

			hasColorMap = header[1] == 1;
			isRleCompressed = header[2] > 8;

			if(header[2] == 0){
				burnWarn("Cannot load TarGA '" + filename
				+ "'! No image data is present.");
				return false;
			}

			// Check for greyscale. This is not supported
			if(header[2] == 3 || header[2] == 11){
				burnWarn("Cannot load TarGA '" + filename
				+ "'! Greyscale images are not supported.");
				return false;
			}

			// Double check with image type
			if(hasColorMap){

				if(!isRleCompressed){
					if(header[2] != 1){    // = uncompressed color mapped
						burnWarn("Cannot load TarGA '" + filename
						+ "'! Corrupt header data.");
						return false;
					}
				}else if(header[2] != 9){    // = RLE color mapped
					burnWarn("Cannot load TarGA '" + filename
					+ "'! Corrupt header data.");
					return false;
				}

				// Get more information from the header
				colorMapPos = header[3] * 0x100 + header[4];
				//colorMapSize = header[5] * 0x100 + header[6];
				colorMapBpp = header[7];

			}else{
				// true-color

				if(!isRleCompressed){
					if(header[2] != 2){    // = uncompressed true-color
						burnWarn("Cannot load TarGA '" + filename
						+ "'! Corrupt header data.");
						return false;
					}
				}else if(header[2] != 10){    // = RLE true-color
					burnWarn("Cannot load TarGA '" + filename
					+ "'! Corrupt header data.");
					return false;
				}

			}

			// Get more information from the header
			width = (header[13] * 0x100 + header[12])
			- (header[9] * 0x100 + header[8]);
			height = (header[15] * 0x100 + header[14])
			- (header[11] * 0x100 + header[10]);
			imageBpp = header[16];

			// Check bpp
			if(hasColorMap){
				if(colorMapBpp != 24 && colorMapBpp != 32){
					burnWarn("Cannot load TarGA '" + filename
					+ "'! Only 24-/32-bit images supported.");
					return false;
				}

				imageSize = width * height * (colorMapBpp / 8);

			}else{
				if(imageBpp != 24 && imageBpp != 32){
					burnWarn("Cannot load TarGA '" + filename
					+ "'! Only 24-/32-bit images supported.");
					return false;
				}

				imageSize = width * height * (imageBpp / 8);

			}

			std::cout << "Image information: Type=TGA, Format=" << width
			<< "*" << height << "*" << Uint32(hasColorMap ?
			colorMapBpp : imageBpp) << ", RLE=" << isRleCompressed
			<< ", ColorMap=" << hasColorMap << ", Size=" << imageSize << "\n";

			// Load data
			Uint32 bytes = 0;
			Uint32 start = file.tellg();
			;
			if(!hasColorMap){
				file.seekg(0, std::ios::end);
				Uint32 end = file.tellg();
				file.seekg(start, std::ios::beg);
				bytes = end - start;
			}else{
				bytes = colorMapPos - start;
			}

			Uint8* data = new Uint8[bytes];
			if(!file.read(reinterpret_cast<char*>(data), bytes)){
				burnWarn("Cannot load TarGA '" + filename
				+ "'! Failed to load image data.");
				delete[] data;
				return false;
			}

			// Load the pixels
			if(!isRleCompressed){
				// Load uncompressed data

				if(!hasColorMap){
					// true-color data

					// Use data directly
					Texture::DataFormat dataFormat;
					if(imageBpp == 32)
						dataFormat = Texture::BGRA;
					else
						dataFormat = Texture::BGR;

					texture.loadFromData(	Vector2ui(width, height),
											Texture::RGB,
											dataFormat,
											Texture::UNSIGNED_BYTE,
											data);
					return true;

				}

			}else{
				// Load RLE compressed data

				if(!hasColorMap){
					// true-color data

					Uint8* pixels = new Uint8[imageSize];
					Uint32 index = 0;

					Uint8 pixelSize = imageBpp / 8;    // Bytes per pixel
					Uint8* cur = &data[0];    // Cursor

					while(cur < &data[bytes - 1]){

						// Get RLE header
						if(*cur & 0x80){
							// RLE chunk

							Uint8 length = *cur - 127;
							++cur;

							for(Uint8 i = 0; i != length;
							++i, index += pixelSize){
								memcpy(&pixels[index], cur, pixelSize);
							}

							cur += pixelSize;

						}else{
							// Raw chunk

							Uint8 length = *cur + 1;
							++cur;

							for(Uint8 i = 0; i != length;
							++i, index += pixelSize, cur += pixelSize){
								memcpy(&pixels[index], cur, pixelSize);
							}

						}

					}

					Texture::DataFormat dataFormat;
					if(imageBpp == 32)
						dataFormat = Texture::BGRA;
					else
						dataFormat = Texture::BGR;

					texture.loadFromData(	Vector2ui(width, height),
											Texture::RGB,
											dataFormat,
											Texture::UNSIGNED_BYTE,
											pixels);

					delete[] pixels;

				}

			}

			delete[] data;

			return true;
		}

	} /* namespace priv */
} /* namespace burn */
