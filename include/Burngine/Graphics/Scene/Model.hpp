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

#ifndef MODEL_HPP_
#define MODEL_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Transformable3D.hpp>
#include <Burngine/Graphics/Scene/Mesh.hpp>
#include <Burngine/Graphics/Scene/Instance.hpp>
#include <vector>
#include <string>

namespace burn {

	/**
	 * @brief Container that holds several models and renderable instances
	 */
	class BURNGINE_API_EXPORT Model : public Transformable3D {
	public:

		bool loadFromFile(const std::string& file);

	private:
		std::vector<Mesh*> m_meshes;	///< All meshes of this model
		std::vector<Material*> m_materials; ///< All used materials
		std::vector<Instance*> m_instances;
	};

} /* namespace burn */

#endif /* MODEL_HPP_ */
