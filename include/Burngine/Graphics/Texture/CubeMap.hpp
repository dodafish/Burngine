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

#ifndef INCLUDE_BURNGINE_GRAPHICS_TEXTURE_CUBEMAP_HPP_
#define INCLUDE_BURNGINE_GRAPHICS_TEXTURE_CUBEMAP_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Texture/BaseTexture.hpp>

namespace burn {

	class BURNGINE_API_EXPORT CubeMap : public BaseTexture {
	public:

		/**
		 * @brief Load a texture from file with TextureLoader
		 *
		 * @param file Relative path to the texture
		 *
		 * @return True if texture was loaded successfully
		 */
		bool loadFromFile(	const std::string& fileXPos,
							const std::string& fileXNeg,
							const std::string& fileYPos,
							const std::string& fileYNeg,
							const std::string& fileZPos,
							const std::string& fileZNeg);

		/**
		 * @brief Bind the texture to a specific unit
		 */
		virtual void bind(const Uint32& unit) const;

	private:

	};

} /* namespace burn */

#endif /* INCLUDE_BURNGINE_GRAPHICS_TEXTURE_CUBEMAP_HPP_ */
