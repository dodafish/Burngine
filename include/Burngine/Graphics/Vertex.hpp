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
#ifndef VERTEX_HPP_
#define VERTEX_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/System/Math.hpp>

namespace burn {

	/**
	 * @brief Holds information about a vertex. I.e. position, normal and uv
	 */
	class BURNGINE_API_EXPORT Vertex {
	public:

		/**
		 * @brief Default constructor with optional initial
		 * values
		 */
		Vertex(	const Vector3f& position = Vector3f(0.f),
				const Vector3f& normal = Vector3f(1.f));

		/**
		 * @brief Set position coordinates
		 */
		void setPosition(const Vector3f& position);

		/**
		 * @brief Get position coordinates
		 */
		const Vector3f& getPosition() const;

		/**
		 * @brief Set normal coordinates
		 */
		void setNormal(const Vector3f& normal);

		/**
		 * @brief Get normal coordinates
		 */
		const Vector3f& getNormal() const;

		/**
		 * @brief Set uv coordinates for specific uv channel
		 */
		void setUv(const Vector2f& uv, const Uint32& channel);

		/**
		 * @brief Get uv coordinates
		 */
		const Vector2f& getUv(const Uint32& channel) const;

		void setTangents(const Vector3f& tangent, const Vector3f& bitangent);

		const Vector3f& getTangent() const;

		const Vector3f& getBitangent() const;

	private:
		Vector3f m_position;
		Vector3f m_normal;
		Vector2f m_uv[8];
		Vector3f m_tangents[2]; ///< Tangent and Bitangent
	};

} /* namespace burn */

#endif /* VERTEX_HPP_ */
