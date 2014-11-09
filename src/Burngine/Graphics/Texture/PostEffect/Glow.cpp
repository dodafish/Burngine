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

#include <Burngine/Graphics/Texture/PostEffect/Glow.hpp>

namespace burn {

	void Glow::apply(Texture& texture) {

		if(!m_texture.isLoaded()
		|| m_texture.getDimensions() != texture.getDimensions()){
			m_texture.loadFromData(	texture.getDimensions(),
									Texture::RGBA,
									Texture::DATA_RGBA,
									0);
			m_framebufferExtract.create(m_texture.getDimensions(), false, m_texture);
		}

		if(m_framebufferExtract.prepare()){

			// TODO Extract pixels

		}

	}

} /* namespace burn */
