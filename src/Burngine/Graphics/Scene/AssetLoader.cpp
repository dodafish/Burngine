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

namespace burn {

	bool AssetLoader::loadAsset(const std::string& file) {

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
		m_materials.clear();

		// Extract the asset's data
		extractMaterials(scene);

		return true;
	}

	void AssetLoader::extractMaterials(const aiScene* assScene) {

		for(unsigned int i = 0; i < assScene->mNumMaterials; ++i){

			aiMaterial* assMat = assScene->mMaterials[i];
			Material* burnMat = new Material();

			//Diffuse
			aiVector3D diffuseColor(0.f);
			assMat->Get(AI_MATKEY_COLOR_DIFFUSE, diffuseColor);
			burnMat->setDiffuseColor(Vector3f(diffuseColor.x, diffuseColor.y, diffuseColor.z));
			//Specular
			aiVector3D specularColor(0.f);
			assMat->Get(AI_MATKEY_COLOR_SPECULAR, specularColor);
			burnMat->setSpecularColor(Vector3f(specularColor.x, specularColor.y, specularColor.z));
			//Ambient
			aiVector3D ambientColor(1.f);
			assMat->Get(AI_MATKEY_COLOR_AMBIENT, specularColor);
			burnMat->setAmbientColor(Vector3f(ambientColor.x, ambientColor.y, ambientColor.z));
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

			// Store mesh
			m_meshes.push_back(burnMesh);

		}

	}

} /* namespace burn */
