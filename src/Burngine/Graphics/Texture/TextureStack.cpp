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

#include <Burngine/Graphics/Texture/TextureStack.hpp>

namespace burn {

	TextureStack::TextureStack() :
	m_baseColor(0.f) {

		for(int i = 0; i != 8; ++i){
			m_textures[i] = NULL;
			m_blendings[i] = 0.f;
			m_operators[i] = OVERWRITE;
		}

	}

	void TextureStack::setBaseColor(const Vector3f& baseColor) {
		m_baseColor = baseColor;
	}
	const Vector3f& TextureStack::getBaseColor() const {
		return m_baseColor;
	}

	void TextureStack::setTexture(	Texture2D* texture,
									const Uint32& channel) {
		m_textures[channel] = texture;
	}
	Texture2D* TextureStack::getTexture(const Uint32& channel) const {
		return m_textures[channel];
	}

	void TextureStack::setBlending(	const float& blending,
									const Uint32& channel) {
		m_blendings[channel] = blending;
	}
	const float& TextureStack::getBlending(const Uint32& channel) const {
		return m_blendings[channel];
	}
	void TextureStack::setOperator(	const Operator& op,
									const Uint32& channel) {
		m_operators[channel] = op;
	}
	const TextureStack::Operator& TextureStack::getOperator(const Uint32& channel) const {
		return m_operators[channel];
	}

	void TextureStack::setUvIndex(	const Int32& uvIndex,
									const Uint32& channel) {
		m_uvIndex[channel] = uvIndex;
	}

	const Int32& TextureStack::getUvIndex(const Uint32& channel) const {
		return m_uvIndex[channel];
	}

} /* namespace burn */
