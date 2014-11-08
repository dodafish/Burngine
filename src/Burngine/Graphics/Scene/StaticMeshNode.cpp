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

#include <Burngine/Graphics/Scene/StaticMeshNode.hpp>

namespace burn {

	void StaticMeshNode::setModel(const Model& model) {
		m_model = model;

		for(size_t i = 0; i < m_model.getMeshes().size(); ++i)
			m_model.getMeshes()[i].setParent(*this);

	}

	void StaticMeshNode::render(const Matrix4f& view,
								const Matrix4f& projection) const {

		for(size_t i = 0; i < m_model.getMeshes().size(); ++i)
			m_model.getMeshes()[i].render(view, projection);

	}

	void StaticMeshNode::render(const Shader& shader) const {

		for(size_t i = 0; i < m_model.getMeshes().size(); ++i)
			m_model.getMeshes()[i].render(shader);
	}

	void StaticMeshNode::renderShadowMap(	const Matrix4f& view,
											const Matrix4f& projection,
											bool useRawZ) const {

		for(size_t i = 0; i < m_model.getMeshes().size(); ++i)
			m_model.getMeshes()[i].renderShadowMap(view, projection, useRawZ);

	}

} /* namespace burn */
