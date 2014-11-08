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

#include <Burngine/Graphics/Texture/PostEffect/Blur.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Gui/Sprite.hpp>

namespace burn {

	Framebuffer Blur::m_framebuffer;
	Texture Blur::m_texture;

	void Blur::apply(Texture& texture) {

		// Framebuffer for first pass towards private texture
		m_framebuffer.create(texture.getDimensions(), false, m_texture);

		if(m_framebuffer.prepare()){

			const Shader& shader = BurnShaders::getShader(BurnShaders::BLUR);
			shader.setUniform("gIsSecondPass", false);
			shader.setUniform("gBlurWidth", 20.f);

			Sprite s;
			s.setTexture(texture, true);
			s.render(Matrix4f(1.f), Matrix4f(1.f), &shader);


		}

	}

} /* namespace burn */
