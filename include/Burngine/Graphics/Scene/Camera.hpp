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

#ifndef CAMERA_HPP_
#define CAMERA_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Transformable3D.hpp>

namespace burn {

	/**
	 * @brief The viewpoint for rendering scenes
	 */
	class BURNGINE_API_EXPORT Camera : public Transformable3D {
	public:

		/**
		 * @brief Sets standard values or initial values
		 * set by user
		 */
		Camera(	const float& aspectRatio = (16.f / 9.f),
				const float& fov = 50.f);

		/**
		 * @brief Set the aspect ratio
		 *
		 * @param ratio The aspect ratio (e.g. 1.6f for 16:10 aspect)
		 */
		void setAspectRatio(const float& ratio);

		/**
		 * @brief Get the aspect ratio
		 *
		 * @return Camera's aspect ratio
		 */
		const float& getAspectRatio() const;

		/**
		 * @brief Set the camera's rotation by looking at a point
		 *
		 * @param focus Focus point
		 */
		void lookAt(const Vector3f& focus);

		/**
		 * @brief Set the camera's field of view
		 *
		 * @param fov field of view
		 */
		void setFov(const float& fov);

		/**
		 * @brief Get the camera's field of view
		 *
		 * @return Current field of view
		 */
		const float& getFov() const;

	private:
		float m_aspectRatio;    ///< Screens ratio (e.g. 16:9)
		float m_fieldOfView;    ///< FOV
	};

} /* namespace burn */

#endif /* CAMERA_HPP_ */
