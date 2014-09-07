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
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>

namespace burn {

	void StaticMeshNode::setModel(const Model& model) {
		m_model = model;
	}

	void StaticMeshNode::render(const Matrix4f& view,
								const Matrix4f& projection) const {

		ensureContext();

		bindVertexArray();

		for(size_t i = 0; i < m_model.getMeshes().size(); ++i){

			const Mesh& mesh = m_model.getMeshes()[i];

			if(mesh.getMaterial().getDiffuseTexture().isLoaded()){

				glEnableVertexAttribArray(0);
				glEnableVertexAttribArray(2);
				m_model.getMeshes()[i].getVertexBuffer().bind();
				glVertexAttribPointer(	0,
										3,
										GL_FLOAT,
										GL_FALSE,
										sizeof(Vector3f) + sizeof(Vector3f)
										+ sizeof(Vector2f),
										(void*)0);
				glVertexAttribPointer(	2,
										2,
										GL_FLOAT,
										GL_FALSE,
										sizeof(Vector3f) + sizeof(Vector3f)
										+ sizeof(Vector2f),
										(void*)(sizeof(Vector3f)
										+ sizeof(Vector3f)));

				const Shader& shader =
				BurnShaders::getShader(BurnShaders::TEXTURE);
				shader.resetTextureUnitCounter();
				shader.setUniform("gModelMatrix", getModelMatrix());
				shader.setUniform("gViewMatrix", view);
				shader.setUniform("gProjectionMatrix", projection);
				shader.setUniform("gColor", Vector4f(1.f));
				shader.bindTexture(	"gTextureSampler",
									mesh.getMaterial().getDiffuseTexture());
				shader.activate();

			}else{

				glEnableVertexAttribArray(0);
				m_model.getMeshes()[i].getVertexBuffer().bind();
				glVertexAttribPointer(	0,
										3,
										GL_FLOAT,
										GL_FALSE,
										sizeof(Vector3f) + sizeof(Vector3f)
										+ sizeof(Vector2f),
										(void*)0);

				const Shader& shader =
				BurnShaders::getShader(BurnShaders::COLOR);

				shader.resetTextureUnitCounter();

				shader.setUniform("gModelMatrix", getModelMatrix());
				shader.setUniform("gViewMatrix", view);
				shader.setUniform("gProjectionMatrix", projection);
				shader.setUniform(	"gColor",
									Vector4f(	mesh.getMaterial().getDiffuseColor(),
												1.f));
				shader.activate();
			}

			glDrawArrays( GL_TRIANGLES, 0, mesh.getVertexCount());

		}

		unbindVertexArray();

	}

	void StaticMeshNode::onVertexArrayCreation() const {
		// Upload data and set pointer

	}

} /* namespace burn */
