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

#ifndef MATERIAL_HPP_
#define MATERIAL_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <Burngine/Graphics/Texture/TextureStack.hpp>

namespace burn {

	/**
	 * @brief Defines the material (appearance) of an object, i.e. Mesh
	 */
	class BURNGINE_API_EXPORT Material {
	public:

		/**
		 * @brief Sets default values
		 */
		Material();

		void setShininess(const float& shininess);

		const float& getShininess() const;

		void setTextureStack(const TextureStack& stack);

		const TextureStack& getTextureStack() const;

	private:
		TextureStack m_textureStack;	///< ATM only diffuse stack
		float m_shininess;    ///< Hardness of specular lighting
	};

} /* namespace burn */

#endif /* MATERIAL_HPP_ */
