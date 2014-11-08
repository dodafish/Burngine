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

#include <Burngine/Graphics/Scene/Mesh.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>

namespace burn {

	Mesh::Mesh() :
	m_vertexCount(0) {

	}

	bool Mesh::addData(	const Vertex* vertices,
						const Uint32& size,
						const Material& material) {

		// Check input:
		// Pointer has to be valid
		// Mesh has to be at least one triangle = 3 vertices
		if(vertices == NULL || size < 3){
			burnWarn("Cannot load mesh! Vertices are invalid or too small.");
			return false;
		}

		VertexMaterial vm;
		vm.first = m_vertexCount;
		vm.count = size;
		vm.material = material;
		m_vertexMaterials.push_back(vm);

		// Add the vertices
		for(Uint32 i = 0; i < size; ++i){
			m_vertexBuffer.addData(	&((vertices + i)->getPosition()),
									sizeof(Vector3f));
			m_vertexBuffer.addData(	&((vertices + i)->getNormal()),
									sizeof(Vector3f));
			m_vertexBuffer.addData(	&((vertices + i)->getUv()),
									sizeof(Vector2f));
		}

		m_vertexCount += size;

		return true;
	}

	void Mesh::setMaterial(const Material& material) {

		size_t totalCount = 0;
		for(size_t i = 0; i < m_vertexMaterials.size(); ++i){
			totalCount += m_vertexMaterials[i].count;
		}
		m_vertexMaterials.clear();

		VertexMaterial vm;
		vm.first = 0;
		vm.count = totalCount;
		vm.material = material;

		m_vertexMaterials.push_back(vm);
	}

	void Mesh::render(	const Matrix4f& view,
						const Matrix4f& projection) const {

		ensureContext();
		checkVertexArray();

		m_vertexArray.bind();
		for(size_t i = 0; i < m_vertexMaterials.size(); ++i){

			if(m_vertexMaterials[i].material.getDiffuseTexture().isLoaded()){
				const Shader& shader = BurnShaders::getShader(BurnShaders::TEXTURE);
				shader.resetTextureUnitCounter();
				shader.setUniform("gModelMatrix", getModelMatrix());
				shader.setUniform("gViewMatrix", view);
				shader.setUniform("gProjectionMatrix", projection);
				shader.setUniform("gColor", Vector4f(1.f));
				shader.bindTexture(	"gTextureSampler",
									m_vertexMaterials[i].material.getDiffuseTexture());
				shader.activate();
			}else{
				const Shader& shader = BurnShaders::getShader(BurnShaders::COLOR);
				shader.resetTextureUnitCounter();
				shader.setUniform("gModelMatrix", getModelMatrix());
				shader.setUniform("gViewMatrix", view);
				shader.setUniform("gProjectionMatrix", projection);
				shader.setUniform(	"gColor",
									Vector4f(	m_vertexMaterials[i].material.getDiffuseColor(),
												1.f));
				shader.activate();
			}

			glDrawArrays( 	GL_TRIANGLES,
							m_vertexMaterials[i].first,
							m_vertexMaterials[i].count);
		}
		m_vertexArray.unbind();

	}

	void Mesh::render(const Shader& shader) const {

		ensureContext();
		checkVertexArray();

		m_vertexArray.bind();
		for(size_t i = 0; i < m_vertexMaterials.size(); ++i){

			shader.activate();

			glDrawArrays( 	GL_TRIANGLES,
							m_vertexMaterials[i].first,
							m_vertexMaterials[i].count);
		}
		m_vertexArray.unbind();

	}

	void Mesh::renderShadowMap(	const Matrix4f& view,
								const Matrix4f& projection,
								bool useRawZ) const {

		ensureContext();
		checkVertexArray();

		m_vertexArray.bind();
		for(size_t i = 0; i < m_vertexMaterials.size(); ++i){

			const Shader& shader = BurnShaders::getShader(BurnShaders::VSM);
			shader.setUniform("gModelMatrix", getModelMatrix());
			shader.setUniform("gViewMatrix", view);
			shader.setUniform("gProjectionMatrix", projection);
			shader.setUniform("gUseRawZ", useRawZ ?
			GL_TRUE : GL_FALSE);
			shader.activate();

			glDrawArrays( 	GL_TRIANGLES,
							m_vertexMaterials[i].first,
							m_vertexMaterials[i].count);
		}
		m_vertexArray.unbind();

	}

	void Mesh::checkVertexArray() const {

		if(m_vertexArray.needsUpdate()){
			m_vertexArray.bind();
			glEnableVertexAttribArray(0);    // Position
			glEnableVertexAttribArray(1);    // Normal
			glEnableVertexAttribArray(2);    // UV
			m_vertexBuffer.bind();
			glVertexAttribPointer(	0,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector3f)
									+ sizeof(Vector2f),
									(void*)0);
			glVertexAttribPointer(	1,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector3f)
									+ sizeof(Vector2f),
									(void*)sizeof(Vector3f));
			glVertexAttribPointer(	2,
									2,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector3f)
									+ sizeof(Vector2f),
									(void*)(sizeof(Vector3f)
									+ sizeof(Vector3f)));
			m_vertexArray.unbind();

			m_vertexArray.setUpdated();
		}

	}

} /* namespace burn */
