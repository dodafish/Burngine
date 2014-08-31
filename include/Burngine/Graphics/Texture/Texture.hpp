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

#ifndef TEXTURE_HPP_
#define TEXTURE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/OpenGL.hpp>
#include <string>

namespace burn {

	/**
	 * @brief Reference to a texture loaded by TextureLoader
	 */
	class BURNGINE_API_EXPORT Texture : public GlEntity {
	public:

		/**
		 * @brief Initialize
		 */
		Texture();

		/**
		 * @brief Load a texture from file with TextureLoader
		 *
		 * @param file Relative path to the texture
		 *
		 * @return True if texture was loaded successfully
		 */
		bool loadFromFile(const std::string& file);

		/**
		 * @brief Get the OpenGL ID. For internal use.
		 */
		const GLuint& getId() const;

	private:
		GLuint m_id;    ///< Texture ID in OpenGL
	};

} /* namespace burn */

#endif /* TEXTURE_HPP_ */
