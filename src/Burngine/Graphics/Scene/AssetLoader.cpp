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

#include <Burngine/Graphics/Scene/AssetLoader.hpp>
#include <Burngine/System/Error.hpp>
#include <iostream>

namespace burn {

	std::vector<Material*> AssetLoader::m_materials;
	std::vector<Mesh*> AssetLoader::m_meshes;
	std::vector<Instance*> AssetLoader::m_instances;    ///< Node tree

	bool AssetLoader::loadAsset(const std::string& file) {

		// Assimp importer instance
		Assimp::Importer importer;

		// Load and process the asset
		const aiScene* scene = importer.ReadFile(	file.c_str(),
													aiProcess_CalcTangentSpace | aiProcess_Triangulate
													| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
													| aiProcess_LimitBoneWeights | aiProcess_GenNormals
													| aiProcess_RemoveRedundantMaterials
													| aiProcess_GenUVCoords | aiProcess_FindInstances
													);

		// Check for success
		if(!scene){
			burnWarn(importer.GetErrorString());
			return false;
		}

		// Reset data
		m_instances.clear();
		m_meshes.clear();
		m_materials.clear();

		// Extract the asset's data. The order is important! Keep it!
		extractMaterials(scene);    // Independant
		extractMeshes(scene);    // Meshes depend on materials
		// Scan through nodes (depend on meshes)
		extractNodes(scene->mRootNode, NULL);

		std::cout << "Asset loaded: " << m_materials.size() << " Materials, " << m_meshes.size()
		<< " Meshes, " << m_instances.size() << " Instances.\n";

		return true;
	}

	const std::vector<Material*>& AssetLoader::getMaterials() {
		return m_materials;
	}
	const std::vector<Mesh*>& AssetLoader::getMeshes() {
		return m_meshes;
	}
	const std::vector<Instance*>& AssetLoader::getInstances() {
		return m_instances;
	}

	void AssetLoader::extractMaterials(const aiScene* assScene) {

		for(unsigned int i = 0; i < assScene->mNumMaterials; ++i){

			aiMaterial* assMat = assScene->mMaterials[i];
			Material* burnMat = new Material();

			//Diffuse
			aiColor3D diffuseColor(0.f);
			assMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			burnMat->setDiffuseColor(Vector3f(diffuseColor.r, diffuseColor.g, diffuseColor.b));
			//Specular
			aiColor3D specularColor(0.f);
			assMat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
			burnMat->setSpecularColor(Vector3f(specularColor.r, specularColor.g, specularColor.b));
			//Ambient
			aiColor3D ambientColor(0.f);
			assMat->Get(AI_MATKEY_COLOR_AMBIENT, specularColor);
			burnMat->setAmbientColor(Vector3f(ambientColor.r, ambientColor.g, ambientColor.b));
			//Shininess
			float shininess = 1.f;
			assMat->Get(AI_MATKEY_SHININESS, shininess);
			burnMat->setShininess(shininess);

			// Store material
			m_materials.push_back(burnMat);

		}

	}

	void AssetLoader::extractMeshes(const aiScene* assScene) {

		for(unsigned int i = 0; i < assScene->mNumMeshes; ++i){

			aiMesh* assMesh = assScene->mMeshes[i];
			Mesh* burnMesh = new Mesh();

			////////////////////////
			// Extract Vertex Data:
			std::vector<Vertex> vertices;    // Vertices for one face
			for(unsigned int f = 0; f < assMesh->mNumFaces; ++f){
				const aiFace& face = assMesh->mFaces[f];
				for(unsigned int fv = 0; fv < face.mNumIndices; ++fv){
					Vertex v;    // One vertex in face

					unsigned int index = face.mIndices[fv];

					//Now fetch the vertex data
					aiVector3D pos = assMesh->mVertices[index];
					aiVector3D norm = assMesh->mNormals[index];
					aiVector3D uv(0.f);
					if(assMesh->HasTextureCoords(0))
						uv = assMesh->mTextureCoords[0][index];

					v.setPosition(Vector3f(pos.x, pos.y, pos.z));
					v.setNormal(Vector3f(norm.x, norm.y, norm.z));
					v.setUv(Vector2f(uv.x, uv.y));

					vertices.push_back(v);
				}
			}
			burnMesh->addData(&vertices[0], vertices.size());    // Store face's vertices

			// Find right material:
			burnMesh->setMaterial(m_materials[assMesh->mMaterialIndex]);

			// Store mesh
			m_meshes.push_back(burnMesh);

		}

	}

	void AssetLoader::extractNodes(	aiNode* node,
									Instance* targetParent) {

		Instance* newInstance = new Instance();
		newInstance->setParent(targetParent);

		Matrix4f transform;
		for(int i = 0; i != 4; ++i)
			for(int j = 0; j != 4; ++j)
				transform[i][j] = node->mTransformation[i][j];
		newInstance->setOffsetMatrix(transform);

		for(unsigned int i = 0; i < node->mNumMeshes; ++i){
			unsigned int index = node->mMeshes[i];
			newInstance->addMesh(m_meshes[index]);
		}

		m_instances.push_back(newInstance);

		// Scan children recursively
		for(unsigned int i = 0; i < node->mNumChildren; ++i)
			extractNodes(node->mChildren[i], newInstance);

	}

} /* namespace burn */
