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
	m_material(NULL),
	m_indexCount(0),
	m_vertexCount(0),
	m_renderTechnique(PLAIN) {

	}

	bool Mesh::addData(	const Vertex* vertices,
						const Uint32& size) {

		// Check input:
		// Pointer has to be valid
		// Mesh has to be at least one triangle = 3 vertices
		if(vertices == NULL || size < 3){
			burnWarn("Cannot load mesh! Vertices are invalid or too little.");
			return false;
		}

		// Add the vertices
		for(Uint32 i = 0; i < size; ++i){
			m_vertexBuffer.addData(&((vertices + i)->getPosition()), sizeof(Vector3f));
			m_vertexBuffer.addData(&((vertices + i)->getNormal()), sizeof(Vector3f));
			m_vertexBuffer.addData(&((vertices + i)->getUv()), sizeof(Vector2f));
		}

		m_vertexCount += size;

		return true;
	}

	void Mesh::setMaterial(Material* material) {
		m_material = material;
	}

	void Mesh::setIndices(const std::vector<unsigned short>& indices) {
		m_indexBuffer.reset();
		m_indexBuffer.addData(&indices[0], indices.size());
		m_indexCount = indices.size();
	}

	void Mesh::render(	const Matrix4f& model,
						const Matrix4f& view,
						const Matrix4f& projection) const {

		//if(!m_material)
		//	return;

		ensureContext();
		checkVertexArray();

		m_vertexArray.bind();

		/*if(m_material->getDiffuseTexture().isLoaded()){
		 const Shader& shader = BurnShaders::getShader(BurnShaders::TEXTURE);
		 shader.resetTextureUnitCounter();
		 shader.setUniform("gModelMatrix", model);
		 shader.setUniform("gViewMatrix", view);
		 shader.setUniform("gProjectionMatrix", projection);
		 shader.setUniform("gColor", Vector4f(1.f));
		 shader.bindTexture("gTextureSampler", m_material->getDiffuseTexture());
		 shader.activate();
		 }else{*/
		const Shader& shader = BurnShaders::getShader(BurnShaders::COLOR);
		shader.resetTextureUnitCounter();
		shader.setUniform("gModelMatrix", model);
		shader.setUniform("gViewMatrix", view);
		shader.setUniform("gProjectionMatrix", projection);
		shader.setUniform("gColor", Vector4f(m_material->getDiffuseColor(), 1.f));
		shader.activate();
		//}

		if(m_renderTechnique == PLAIN){
			glDrawArrays( GL_TRIANGLES, 0, m_vertexCount);
		}else{
			m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
			glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, (void*)0);
		}

		m_vertexArray.unbind();

	}

	void Mesh::render(const Shader& shader) const {

		ensureContext();
		checkVertexArray();

		shader.activate();

		m_vertexArray.bind();
		if(m_renderTechnique == PLAIN){
			glDrawArrays( GL_TRIANGLES, 0, m_vertexCount);
		}else{
			m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
			glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, (void*)0);
		}
		m_vertexArray.unbind();

	}

	void Mesh::renderShadowMap(	const Matrix4f& model,
								const Matrix4f& view,
								const Matrix4f& projection,
								bool useRawZ) const {

		ensureContext();
		checkVertexArray();

		m_vertexArray.bind();

		const Shader& shader = BurnShaders::getShader(BurnShaders::VSM);
		shader.setUniform("gModelMatrix", model);
		shader.setUniform("gViewMatrix", view);
		shader.setUniform("gProjectionMatrix", projection);
		shader.setUniform("gUseRawZ", useRawZ ?
		GL_TRUE :
		GL_FALSE);
		shader.activate();

		if(m_renderTechnique == PLAIN){
			glDrawArrays( GL_TRIANGLES, 0, m_vertexCount);
		}else{
			m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
			glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_SHORT, (void*)0);
		}

		m_vertexArray.unbind();

	}

	void Mesh::setRenderTechnique(const RenderTechnique& renderTechnique) {
		m_renderTechnique = renderTechnique;
	}

	void Mesh::checkVertexArray() const {

		if(m_vertexArray.needsUpdate()){
			m_vertexArray.bind();

			m_indexBuffer glEnableVertexAttribArray(0);    // Position
			glEnableVertexAttribArray(1);    // Normal
			glEnableVertexAttribArray(2);    // UV
			m_vertexBuffer.bind();
			glVertexAttribPointer(	0,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector3f) + sizeof(Vector2f),
									(void*)0);
			glVertexAttribPointer(	1,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector3f) + sizeof(Vector2f),
									(void*)sizeof(Vector3f));
			glVertexAttribPointer(	2,
									2,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) + sizeof(Vector3f) + sizeof(Vector2f),
									(void*)(sizeof(Vector3f) + sizeof(Vector3f)));
			m_vertexArray.unbind();

			m_vertexArray.setUpdated();
		}

	}

} /* namespace burn */
