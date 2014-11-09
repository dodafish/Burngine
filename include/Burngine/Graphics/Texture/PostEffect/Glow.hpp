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

#ifndef GLOW_HPP_
#define GLOW_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Texture/Framebuffer.hpp>
#include <Burngine/System/NonCopyable.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/Graphics/Texture/PostEffect/Blur.hpp>

namespace burn {

	class BURNGINE_API_EXPORT Glow : public NonCopyable {
	public:

		/**
		 * @brief Apply automated glow effect
		 */
		void apply(Texture& texture);

	private:
		Framebuffer m_framebufferExtract;
		Texture m_texture;
		Blur m_blur;
	};

} /* namespace burn */

#endif /* GLOW_HPP_ */
