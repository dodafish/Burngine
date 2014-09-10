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

#ifndef POINTLIGHT_HPP_
#define POINTLIGHT_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Transformable3D.hpp>
#include <Burngine/System/Math.hpp>

namespace burn {

	/**
	 * @brief Lightsource that shines into all directions.
	 * Like a light bulb.
	 */
	class BURNGINE_API_EXPORT PointLight : public Transformable3D {
	public:

		/**
		 * @brief Sets default values
		 */
		PointLight();

		/**
		 * @brief Set the light's color
		 */
		void setColor(const Vector3f& color);

		/**
		 * @brief Get the light's color
		 */
		const Vector3f& getColor() const;

		/**
		 * @bright Set the 'brightness'
		 */
		void setIntensity(const float& intensity);

		/**
		 * @brief Get the 'brightness'
		 */
		const float& getIntensity() const;

	private:
		Vector3f m_color; ///< Light color
		float m_intensity; ///< Light intensity
	};

} /* namespace burn */

#endif /* POINTLIGHT_HPP_ */
