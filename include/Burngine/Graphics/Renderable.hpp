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

#ifndef RENDERABLE_HPP_
#define RENDERABLE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>

namespace burn {

	/**
	 * @brief Base class for all renderable contents. Holds its own VAO
	 */
	class BURNGINE_API_EXPORT Renderable : public GlEntity {
	public:

		/**
		 * @brief Releases VAOs
		 */
		virtual ~Renderable();

		/**
		 * @brief Render the object normally
		 *
		 * @param projection Used projection matrix
		 */
		virtual void render(const Matrix4f& view,
							const Matrix4f& projection) const = 0;

		virtual void render(const Shader& shader) const = 0;

	};

} /* namespace burn */

#endif /* RENDERABLE_HPP_ */
