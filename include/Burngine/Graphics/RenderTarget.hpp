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

#ifndef RENDERTARGET_HPP_
#define RENDERTARGET_HPP_

#include <Burngine/Export.hpp>

namespace burn {

	/**
	 * @brief Parent class for all render targets like windows
	 * or render textures.
	 * Needed for Renderer.
	 */
	class BURNGINE_API_EXPORT RenderTarget {
	public:
		/**
		 * @brief Virtual dtor for derived classes
		 */
		virtual ~RenderTarget();

		/**
		 * @brief Prepare this class for being the target of
		 * rendering.
		 *
		 * @return True if target is ready for rendering. False if
		 * the target cannot be used for rendering at the moment.
		 */
		virtual bool prepare() = 0;
	};

} /* namespace burn */

#endif /* RENDERTARGET_HPP_ */
