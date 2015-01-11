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
#include <sstream>

namespace burn {

	Mesh::Mesh() :
	m_material(
	NULL),
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
			m_vertexBuffer.addData(&((vertices + i)->getTangent()), sizeof(Vector3f));
			m_vertexBuffer.addData(&((vertices + i)->getBitangent()), sizeof(Vector3f));
		}

		m_vertexCount += size;

		return true;
	}

	void Mesh::setMaterial(Material* material) {
		m_material = material;
	}

	void Mesh::setIndices(const std::vector<unsigned short>& indices) {
		m_indexBuffer.reset();
		m_indexBuffer.addData(&indices[0], sizeof(unsigned short) * indices.size());
		m_indexCount = indices.size();
	}

	void Mesh::render(	const Matrix4f& model,
						const Camera& camera) const {

		// Cannot render without material
		if(!m_material)
			return;

		// Ensure we have a context and mesh data is linked
		ensureContext();
		ensureUpdatedVertexArray();

		// Setup shader
		const Shader& shader = BurnShaders::getShader(BurnShaders::MESH_DIFFUSE);
		shader.resetTextureUnitCounter();
		// Matrices
		shader.setUniform("gModelMatrix", model);
		shader.setUniform("gViewMatrix", camera.getViewMatrix());
		shader.setUniform("gProjectionMatrix", camera.getProjectionMatrix());
		// Diffuse color/texture
		if(m_material->getType() == Material::TEXTURED && m_material->getDiffuseTexture().isLoaded()){
			shader.setUniform("gUseDiffuseTexture", true);
			shader.bindTexture("gDiffuseTexture", m_material->getDiffuseTexture());
		}else{
			shader.setUniform("gUseDiffuseTexture", false);
			shader.resetTexture("gDiffuseTexture");
			shader.setUniform("gDiffuseColor", m_material->getDiffuseColor());
		}
		// Normal texture (normal map)
		if(m_material->getNormalTexture().isLoaded()){
			shader.setUniform("gUseNormalTexture", true);
			shader.bindTexture("gNormalTexture", m_material->getNormalTexture());
		}else{
			shader.setUniform("gUseNormalTexture", false);
			shader.resetTexture("gNormalTexture");
		}
		// Reflection cube map
		if(m_material->getReflectionCubeMap().isLoaded()){
			shader.setUniform("gUseReflectionCubeMap", true);
			shader.setUniform("gCameraPosition", camera.getPosition());
			shader.bindTexture("gReflectionCubeMap", m_material->getReflectionCubeMap());
		}else{
			shader.setUniform("gUseReflectionCubeMap", false);
			shader.resetTexture("gReflectionCubeMap");
		}

		// Render the mesh
		shader.activate();
		m_vertexArray.bind();
		if(m_renderTechnique == PLAIN){
			glDrawArrays(GL_TRIANGLES, 0, m_vertexCount);
		}else{
			m_indexBuffer.bind(
			GL_ELEMENT_ARRAY_BUFFER);
			glDrawElements(
			GL_TRIANGLES,
							m_indexCount,
							GL_UNSIGNED_SHORT,
							(void*)0);
		}
		m_vertexArray.unbind();

	}

	void Mesh::renderShadowMap(	const Matrix4f& model,
								const Matrix4f& view,
								const Matrix4f& projection,
								bool useRawZ) const {

		ensureContext();
		ensureUpdatedVertexArray();

		m_vertexArray.bind();

		const Shader& shader = BurnShaders::getShader(BurnShaders::VSM);
		shader.setUniform("gModelMatrix", model);
		shader.setUniform("gViewMatrix", view);
		shader.setUniform("gProjectionMatrix", projection);
		shader.setUniform("gUseRawZ", useRawZ);
		shader.activate();

		if(m_renderTechnique == PLAIN){
			glDrawArrays( GL_TRIANGLES, 0, m_vertexCount);
		}else{
			m_indexBuffer.bind(GL_ELEMENT_ARRAY_BUFFER);
			glDrawElements( GL_TRIANGLES, m_indexCount,
			GL_UNSIGNED_SHORT,
							(void*)0);
		}

		m_vertexArray.unbind();

	}

	void Mesh::setRenderTechnique(const RenderTechnique& renderTechnique) {
		m_renderTechnique = renderTechnique;
	}

	void Mesh::ensureUpdatedVertexArray() const {

		if(m_vertexArray.needsUpdate()){
			m_vertexArray.bind();

			// Enable array slots
			glEnableVertexAttribArray(0);    // Position
			glEnableVertexAttribArray(1);    // Normal
			glEnableVertexAttribArray(2);    // UV
			glEnableVertexAttribArray(3);    // Tangent
			glEnableVertexAttribArray(4);    // Bitangent

			// Link slots
			m_vertexBuffer.bind();
			glVertexAttribPointer(0, 3,
			GL_FLOAT,
									GL_FALSE, sizeof(Vector3f) * 4 + sizeof(Vector2f), (void*)0);
			glVertexAttribPointer(	1,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) * 4 + sizeof(Vector2f),
									(void*)sizeof(Vector3f));
			glVertexAttribPointer(	2,
									2,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) * 4 + sizeof(Vector2f),
									(void*)(sizeof(Vector3f) * 2));

			glVertexAttribPointer(	3,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) * 4 + sizeof(Vector2f),
									(void*)(sizeof(Vector3f) * 2 + sizeof(Vector2f)));
			glVertexAttribPointer(	4,
									3,
									GL_FLOAT,
									GL_FALSE,
									sizeof(Vector3f) * 4 + sizeof(Vector2f),
									(void*)(sizeof(Vector3f) * 3 + sizeof(Vector2f)));

			// "Save" vertex array
			m_vertexArray.unbind();
			m_vertexArray.setUpdated();
		}

	}

} /* namespace burn */
