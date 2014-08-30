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
#include <Burngine/OpenGL.hpp>
#include <Burngine/System/Math.hpp>
#include <map>

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

		/**
		 * @brief Called when the VAO has to be updated. I.e. another context
		 * is calling the VAO (not shareable)
		 */
		virtual void updateVao() const = 0;

	private:

		/**
		 * @brief Get a VAO for the current thread/context
		 */
		const GLuint& getVao() const;

	private:
		mutable std::map<void*, GLuint> m_vaoMap;    ///< Per thread/per context VAO
	};

} /* namespace burn */

#endif /* RENDERABLE_HPP_ */
