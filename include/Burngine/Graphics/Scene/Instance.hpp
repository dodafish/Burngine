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

#ifndef INCLUDE_BURNGINE_GRAPHICS_SCENE_INSTANCE_HPP_
#define INCLUDE_BURNGINE_GRAPHICS_SCENE_INSTANCE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Mesh.hpp>
#include <vector>

namespace burn {

	/**
	 * @brief Subnode for Models. Holds references to meshes
	 */
	class BURNGINE_API_EXPORT Instance : public Transformable3D {
	public:

		void addMesh(Mesh* mesh);

		void render(const Matrix4f& view, const Matrix4f& projection);

	private:
		std::vector<Mesh*> m_meshes;
	};

} /* namespace burn */

#endif /* INCLUDE_BURNGINE_GRAPHICS_SCENE_INSTANCE_HPP_ */
