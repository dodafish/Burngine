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

#ifndef TRANSFORMABLE2D_HPP_
#define TRANSFORMABLE2D_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/Math.hpp>

namespace burn {

	/**
	 * @brief Holds information about a 2D object
	 */
	class BURNGINE_API_EXPORT Transformable2D {
	public:

		/**
		 * @brief Set default values
		 */
		Transformable2D();

		/**
		 * @brief Copy values
		 */
		Transformable2D(const Transformable2D& other);

		/**
		 * @brief assign values
		 */
		Transformable2D& operator=(const Transformable2D& other);

		/**
		 * @brief Set new position values
		 *
		 * @param position New values
		 */
		void setPosition(const Vector2f& position);

		/**
		 * @brief Get position values
		 *
		 * @return current position values
		 */
		const Vector2f& getPosition() const;

		/**
		 * @brief Set new rotation values
		 *
		 * @param rotation New values
		 */
		void setRotation(const float& rotation);

		/**
		 * @brief Get rotation values
		 *
		 * @return current rotation values
		 */
		const float& getRotation() const;

		/**
		 * @brief Set new scale values
		 *
		 * @param scale New values
		 */
		void setScale(const Vector2f& scale);

		/**
		 * @brief Get scale values
		 *
		 * @return current scale values
		 */
		const Vector2f& getScale() const;

		/**
		 * @brief Set new origin values
		 *
		 * @param origin New values
		 */
		void setOrigin(const Vector2f& scale);

		/**
		 * @brief Get origin values
		 *
		 * @return current origin values
		 */
		const Vector2f& getOrigin() const;

		/**
		 * @brief Get the model matrix
		 *
		 * @return Model matrix
		 */
		const Matrix4f& getModelMatrix() const;

	private:

		/**
		 * @brief Recalculate model matrix
		 */
		void updateModelMatrix();

	private:
		Vector2f m_position;    ///< Object's position
		Vector2f m_origin;    ///< Object's origin
		float m_rotation;    ///< Object's rotation
		Vector2f m_scale;    ///< Object's scale
		Matrix4f m_modelMatrix;    ///< Model matrix
	};

} /* namespace burn */

#endif /* TRANSFORMABLE2D_HPP_ */
