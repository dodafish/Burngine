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

#ifndef TEXTURELOADER_HPP_
#define TEXTURELOADER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <vector>
#include <map>
#include <iosfwd>

namespace burn {
	namespace priv {

		/**
		 * @brief Loads and handles textures
		 */
		class BURNGINE_API_EXPORT TextureLoader {
		public:

			/**
			 * @brief Load a texture from file and ready it for
			 * OpenGL usage
			 *
			 * @param file Texture file
			 *
			 * @return Texture. Check isLoaded()
			 */
			static bool loadFromFile(	const std::string& file,
										Texture& target);

			/**
			 * @brief Release allocated memory in OpenGL and delete
			 * textures
			 *
			 * @note Texture classes can still have a texture ID, but it
			 * will become invalid
			 */
			static void cleanup();

			/**
			 * @brief Get the number of loaded textures
			 */
			static const size_t& getTextureCount();

		private:

			/**
			 * @brief Supported filetypes
			 */
			enum FileType {
				UNKNOWN,    ///< Unsupported
				BMP,    ///< Windows Bitmap
				TGA    ///< TarGA
			};

			/**
			 * @brief Check what filetype the texture is
			 */
			static FileType checkFileType(const std::string& file);

			/**
			 * @brief Load a Windows Bitmap
			 */
			static bool loadBmp(const std::string& file,
								Texture& texture);

			/**
			 * @brief Load a TarGA
			 */
			static bool loadTga(const std::string& file,
								Texture& texture);

		private:
			static std::vector<std::pair<size_t, Texture*>> m_textures;    ///< Loaded textures
			static size_t m_count;    ///< Number of loaded textures
		};

	} /* namespace priv */
} /* namespace burn */

#endif /* TEXTURELOADER_HPP_ */
