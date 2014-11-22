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
#include <Burngine/Graphics/Scene/AssetLoader.hpp>

namespace burn {

	bool Model::loadFromFile(const std::string& file) {

		if(AssetLoader::loadAsset(file)){
			m_materials = AssetLoader::getMaterials();
			m_meshes = AssetLoader::getMeshes();
			m_instances = AssetLoader::getInstances();

			if(m_instances.size() > 0)
				m_instances[0]->setParent(this);    // Set root node as child of Model

			return true;
		}

		return false;
	}

	void Model::render(	const Matrix4f& view,
						const Matrix4f& projection) const {
		for(size_t i = 0; i < m_instances.size(); ++i){
			m_instances[i]->render(view, projection);
		}
	}

	void Model::render(const Shader& shader) const {
		for(size_t i = 0; i < m_instances.size(); ++i){
			m_instances[i]->render(shader);
		}
	}

	void Model::renderShadowMap(const Matrix4f& view,
								const Matrix4f& projection,
								bool useRawZ) const {
		for(size_t i = 0; i < m_instances.size(); ++i){
			m_instances[i]->renderShadowMap(view, projection, useRawZ);
		}
	}

} /* namespace burn */