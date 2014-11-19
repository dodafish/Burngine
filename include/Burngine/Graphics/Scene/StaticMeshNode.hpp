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

#ifndef STATICMESHNODE_HPP_
#define STATICMESHNODE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Transformable3D.hpp>
#include <Burngine/Graphics/Scene/Mesh.hpp>
#include <vector>

namespace burn {

	/**
	 * @brief Simple single model node
	 */
	class BURNGINE_API_EXPORT StaticMeshNode : public Transformable3D {
	public:

		void addMesh(const Mesh& mesh);

		const std::vector<Mesh>& getMeshes() const;

	private:
		std::vector<Mesh> m_meshes;    ///< Set of meshes
	};

} /* namespace burn */

#endif /* STATICMESHNODE_HPP_ */
