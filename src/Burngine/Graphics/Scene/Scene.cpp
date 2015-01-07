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

#include <Burngine/Graphics/Scene/Scene.hpp>

namespace burn {

	void Scene::attachModel(Model* sceneNode) {
		for(size_t i = 0; i < m_models.size(); ++i)
			if(m_models[i] == sceneNode)
				return;
		m_models.push_back(sceneNode);
	}

	void Scene::detachModel(Model* sceneNode) {
		for(size_t i = 0; i < m_models.size(); ++i)
			if(m_models[i] == sceneNode){
				m_models.erase(m_models.begin() + i);
				return;
			}
	}

	const std::vector<Model*>& Scene::getModels() const {
		return m_models;
	}

	void Scene::attachLight(DirectionalLight* light) {
		for(size_t i = 0; i < m_directionalLights.size(); ++i)
			if(m_directionalLights[i] == light)
				return;
		m_directionalLights.push_back(light);
	}

	void Scene::attachLight(SpotLight* light) {
		for(size_t i = 0; i < m_spotLights.size(); ++i)
			if(m_spotLights[i] == light)
				return;
		m_spotLights.push_back(light);
	}

	void Scene::attachLight(PointLight* light) {
		for(size_t i = 0; i < m_pointLights.size(); ++i)
			if(m_pointLights[i] == light)
				return;
		m_pointLights.push_back(light);
	}

	const std::vector<DirectionalLight*>& Scene::getDirectionalLights() const {
		return m_directionalLights;
	}

	const std::vector<SpotLight*>& Scene::getSpotLights() const {
		return m_spotLights;
	}

	const std::vector<PointLight*> Scene::getPointLights() const {
		return m_pointLights;
	}

	void Scene::attachBillboard(Billboard* billboard) {
		for(size_t i = 0; i < m_billboards.size(); ++i)
			if(m_billboards[i] == billboard)
				return;
		m_billboards.push_back(billboard);
	}

	const std::vector<Billboard*>& Scene::getBillboards() const {
		return m_billboards;
	}

} /* namespace burn */
