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

#ifndef ROTATION_HPP_
#define ROTATION_HPP_

#include <Burngine/Export.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace burn {

	class BURNGINE_API_EXPORT Rotation {
	public:

	private:

		/**
		 * @brief Updates matrix to fit quaterion
		 */
		void updateMatrix();

	private:
		glm::quat m_quaternion;    ///< Rot. represented as quaternion
		Matrix4f m_matrix;    ///< Rot. represented as Matrix
	};

} /* namespace burn */

#endif /* ROTATION_HPP_ */