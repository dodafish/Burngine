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

#include <Burngine/Graphics/Scene/Model.hpp>

namespace burn {

	bool Model::loadFromFile(const std::string& fileName) {
		//return priv::ModelLoader::loadFromFile(fileName, *this);
		return false; // TODO
	}

	void Model::clear() {
		m_meshes.clear();
	}

	void Model::addMesh(const Mesh& mesh) {
		m_meshes.push_back(mesh);
	}

	const std::vector<Mesh>& Model::getMeshes() const {
		return m_meshes;
	}

} /* namespace burn */
