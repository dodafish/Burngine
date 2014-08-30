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
#include <Burngine/Graphics/Transformable3D.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/OpenGL.hpp>

namespace burn {

	/**
	 * @brief Base class for all renderable contents. Holds its own VAO
	 */
	class BURNGINE_API_EXPORT Renderable3D : public GlEntity, public Transformable3D {
	public:

		/**
		 * @brief Creates VAO
		 */
		Renderable3D();

		/**
		 * @brief Creates VAO
		 */
		Renderable3D(const Renderable3D& other);

		/**
		 * @brief Releases VAO
		 */
		virtual ~Renderable3D();

		/**
		 * @brief Render the object
		 *
		 * @param projection Used projection matrix
		 */
		virtual void render(const Matrix4f& projection) const = 0;

	protected:

		/**
		 * @brief Enable the VAO for editing OpenGL parameters, i.e.
		 * set VBOs
		 */
		void bindVao() const;

		/**
		 * @brief Disable the VAO, i.e. save currently set VBOs
		 */
		void unbindVao() const;

	private:
		GLuint m_vao; ///< Vertex array object of this object
	};

} /* namespace burn */

#endif /* RENDERABLE_HPP_ */
