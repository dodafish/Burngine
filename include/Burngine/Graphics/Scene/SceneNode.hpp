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

#ifndef SCENENODE_HPP_
#define SCENENODE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Transformable3D.hpp>
#include <Burngine/Graphics/Renderable.hpp>

namespace burn {

	class BURNGINE_API_EXPORT SceneNode : public Renderable, public Transformable3D {
	public:

		/**
		 * @brief Virtual destructor for virtual methods
		 */
		virtual ~SceneNode();

		/**
		 * @brief Render the object for VSM shadow map
		 *
		 * @param view Light's view
		 * @param projection Light's projection
		 */
		virtual void renderShadowMap(	const Matrix4f& view,
										const Matrix4f& projection,
										bool useRawZ) const = 0;

	private:

	};

} /* namespace burn */

#endif /* SCENENODE_HPP_ */
