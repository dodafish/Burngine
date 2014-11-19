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

#ifndef TRANSFORMABLE3D_HPP_
#define TRANSFORMABLE3D_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/System/Rotation.hpp>
#include <vector>

namespace burn {

	/**
	 * @brief Holds information about a 3D object in space.
	 */
	class BURNGINE_API_EXPORT Transformable3D {
	public:

		/**
		 * @brief Set default values
		 */
		Transformable3D();

		/**
		 * @brief Copy values
		 */
		Transformable3D(const Transformable3D& other);

		/**
		 * @brief assign values
		 */
		Transformable3D& operator=(const Transformable3D& other);

		/**
		 * @brief Set new position values
		 *
		 * @param position New values
		 */
		void setPosition(const Vector3f& position);

		/**
		 * @brief Get position values
		 *
		 * @return current position values
		 */
		const Vector3f& getPosition() const;

		/**
		 * @brief Set new rotation values
		 *
		 * @param rotation New values
		 */
		void setRotation(const Rotation& rotation);

		/**
		 * @brief Get rotation values
		 *
		 * @return current rotation values
		 */
		const Rotation& getRotation() const;

		/**
		 * @brief Set new scale values
		 *
		 * @param scale New values
		 */
		void setScale(const Vector3f& scale);

		/**
		 * @brief Get scale values
		 *
		 * @return current scale values
		 */
		const Vector3f& getScale() const;

		/**
		 * @brief Get the model matrix with accumulated parent's matrix/matrices
		 *
		 * @return Global model matrix
		 */
		Matrix4f getGlobalModelMatrix() const;

		/**
		 * @brief Get the model matrix WITHOUT accumulated parent's matrix/matrices
		 *
		 * @return Local model matrix
		 */
		Matrix4f getLocalModelMatrix() const;

		/**
		 * @brief Make this node to be a child node of a parent node and
		 * accumulate with the parent's transform
		 *
		 * @param parent The new parent node
		 */
		void setParent(Transformable3D& parent);

		/**
		 * @brief Unbind this node from its parent.
		 */
		void unsetParent();

	private:

		/**
		 * @brief Recalculate local model matrix
		 */
		void updateModelMatrix();

	private:
		Vector3f m_position;    ///< Object's position
		Rotation m_rotation;    ///< Object's rotation
		Vector3f m_scale;    ///< Object's scale
		Matrix4f m_modelMatrix;    ///< Model matrix for this transformable only

		Transformable3D& m_parent;    ///< Parent node
	};

} /* namespace burn */

#endif /* TRANSFORMABLE3D_HPP_ */
