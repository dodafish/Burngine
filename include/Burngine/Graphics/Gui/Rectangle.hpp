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

#ifndef RECTANGLE_HPP_
#define RECTANGLE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Gui/GuiNode.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/Graphics/Renderable.hpp>

namespace burn {

	class BURNGINE_API_EXPORT Rectangle : public GuiNode, public Renderable {
	public:

		/**
		 * @brief Sets VAO up
		 */
		Rectangle();

		/**
		 * @brief Set dimensions
		 *
		 * @param dimensions Dimensions
		 */
		void setDimensions(const Vector2f& dimensions);

		/**
		 * @brief Get dimensions
		 *
		 * @return Rectangle's dimensions
		 */
		const Vector2f& getDimensions() const;

		/**
		 * @brief Set new color
		 *
		 * @param color Color
		 */
		void setColor(const Vector4f& color);

		/**
		 * @brief Get current color
		 *
		 * @return Current color
		 */
		const Vector4f& getColor() const;

		/**
		 * @brief Render the rectangle depending on a view and projection matrix
		 *
		 * @param view Used view matrix
		 * @param projection Used projection matrix
		 */
		virtual void render(const Matrix4f& view, const Matrix4f& projection) const;

	protected:

		/**
		 * @brief Update vertex array object if necessary
		 */
		virtual void ensureUpdatedVertexArray() const;

	private:
		Vector2f m_dimensions;
		Vector4f m_color;    ///< Rectangle's color
		VertexBuffer m_vertexBuffer;
	};

} /* namespace burn */

#endif /* RECTANGLE_HPP_ */
