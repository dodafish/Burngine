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
#include <Burngine/System/Error.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace {

	void fetchAsset(const aiScene* scene,
					std::vector<burn::Mesh*>& meshes,
					std::vector<burn::Material*>& materials,
					std::vector<burn::Instance>& instances);

}

namespace burn {

	bool Model::loadFromFile(const std::string& file) {

		// Assimp importer instance
		Assimp::Importer importer;

		// Load and process the asset
		const aiScene* scene = importer.ReadFile(	file.c_str(),
													aiProcess_CalcTangentSpace | aiProcess_Triangulate
													| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
													| aiProcess_LimitBoneWeights);

		// Check for success
		if(!scene){
			burnWarn(importer.GetErrorString());
			return false;
		}

		// Reset data
		m_instances.clear();
		m_meshes.clear();
		m_materials.clear();

		// Extract the asset's data
		fetchAsset(scene, m_meshes, m_materials, m_instances);

		return true;
	}

} /* namespace burn */

namespace {

	void fetchAsset(const aiScene* scene,
					std::vector<burn::Mesh*>& meshes,
					std::vector<burn::Material*>& materials,
					std::vector<burn::Instance>& instances) {

		/*
		 * Process all meshes
		 */
		for(unsigned int i = 0; i < scene->mNumMeshes; ++i){
			aiMesh* assMesh = scene->mMeshes[i];
			burn::Mesh* burnMesh = new burn::Mesh();

			/********************************************************
			 * Vertex Data
			 *********************************************************/
			for(unsigned int j = 0; j < assMesh->mNumFaces; ++j){
				aiFace face = assMesh->mFaces[j];
				std::vector<burn::Vertex> vertices;
				for(unsigned int k = 0; k < face.mNumIndices; ++k){

					burn::Vertex v;
					v.setPosition(burn::Vector3f(	assMesh->mVertices[face.mIndices[k]].x,
													assMesh->mVertices[face.mIndices[k]].y,
													assMesh->mVertices[face.mIndices[k]].z));

					vertices.push_back(v);

				}
				burnMesh->addData(&vertices[0], vertices.size());
				vertices.clear();
			}

			/********************************************************
			 * Material
			 *********************************************************/
			aiMaterial* assMat = scene->mMaterials[assMesh->mMaterialIndex];
			burn::Material* burnMat = new burn::Material();

			//Diffuse
			aiVector3D diffuseColor(0.f);
			assMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			burnMat->setDiffuseColor(burn::Vector3f(diffuseColor.x, diffuseColor.y, diffuseColor.z));

			// Store material
			materials.push_back(burnMat);

			// Store mesh
			burnMesh->setMaterial(burnMat);
			meshes.push_back(burnMesh);
		}

	}

}
