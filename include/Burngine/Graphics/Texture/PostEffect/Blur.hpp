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

#ifndef BLUR_HPP_
#define BLUR_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Texture/Framebuffer.hpp>
#include <Burngine/System/NonCopyable.hpp>
#include <Burngine/Window/GlEntity.hpp>

namespace burn {

	/**
	 * @brief Blurs a texture
	 */
	class BURNGINE_API_EXPORT Blur : public GlEntity, public NonCopyable {
	public:

		/**
		 * @brief Apply blur effect to a texture
		 *
		 * @param texture Texture to blur
		 * @param attachedFramebuffer It is better to have texture already bound to
		 * a framebuffer and to use this instead of binding to a temporary one
		 */
		void apply(	Texture2D& texture,
					Framebuffer* attachedFramebuffer = NULL,
					const float& blurScale = 1.f);

	private:

		Framebuffer m_framebufferFirst;    ///< Framebuffer for first pass
		Framebuffer m_framebufferSecond;	///< Framebuffer for second pass if none is given
		Texture2D m_texture;    ///< Texture for first pass

	};

}
/* namespace burn */

#endif /* BLUR_HPP_ */
