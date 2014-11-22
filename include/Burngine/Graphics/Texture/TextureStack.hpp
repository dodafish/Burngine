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

#ifndef INCLUDE_BURNGINE_GRAPHICS_TEXTURE_TEXTURESTACK_HPP_
#define INCLUDE_BURNGINE_GRAPHICS_TEXTURE_TEXTURESTACK_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>

namespace burn {

	/**
	 * @brief Stack of textures used for material rendering
	 */
	class BURNGINE_API_EXPORT TextureStack {
	public:

		enum Operator {
			MULTIPLY = 0,    ///< a * b
			ADD,	///< a + b
			SUBTRACT,    ///< a - b
			DIVIDE,		///< a / b
			SMOOTH_ADD,    ///< (a+b)-(a*b)
			SIGNED_ADD    ///< a + (b-0.5)
		};

		void setBaseColor(const Vector3f& baseColor);
		const Vector3f& getBaseColor() const;

		void setTexture(Texture* texture,
						const Uint32& channel);
		Texture* getTexture(const Uint32& channel) const;

		void setBlending(	const float& blending,
							const Uint32& channel);
		const float& getBlending(const Uint32& channel) const;

		void setOperator(	const Operator& op,
							const Uint32& channel);
		const Operator& getOperator(const Uint32& channel) const;

	private:
		Vector3f m_baseColor;
		Texture* m_textures[8];
		float m_blendings[8];
		Operator m_operators[8];
	};

} /* namespace burn */

#endif /* INCLUDE_BURNGINE_GRAPHICS_TEXTURE_TEXTURESTACK_HPP_ */
