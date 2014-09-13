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

	void Scene::attachSceneNode(SceneNode* sceneNode) {
		for(size_t i = 0; i < m_sceneNodes.size(); ++i)
			if(m_sceneNodes[i] == sceneNode)
				return;
		m_sceneNodes.push_back(sceneNode);
	}

	void Scene::detachSceneNode(SceneNode* sceneNode) {
		for(size_t i = 0; i < m_sceneNodes.size(); ++i)
			if(m_sceneNodes[i] == sceneNode){
				m_sceneNodes.erase(m_sceneNodes.begin() + i);
				return;
			}
	}

	const std::vector<SceneNode*>& Scene::getSceneNodes() const {
		return m_sceneNodes;
	}

} /* namespace burn */
