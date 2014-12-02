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
#include <climits>
#include <unordered_set>

namespace {
	std::hash<std::string> strHash;
}

namespace burn {

	std::vector<AssetLoader::Asset> AssetLoader::m_loadedAssets;

	std::vector<Material*> AssetLoader::m_materials;
	std::vector<Mesh*> AssetLoader::m_meshes;
	std::vector<Instance*> AssetLoader::m_instances;    ///< Node tree

	void AssetLoader::cleanup() {

		for(size_t i = 0; i < m_loadedAssets.size(); ++i){
			for(size_t j = 0; j < m_loadedAssets[i].materials.size(); ++j)
				delete m_loadedAssets[i].materials[j];
			for(size_t j = 0; j < m_loadedAssets[i].meshes.size(); ++j)
				delete m_loadedAssets[i].meshes[j];
			for(size_t j = 0; j < m_loadedAssets[i].instances.size(); ++j)
				delete m_loadedAssets[i].instances[j];
		}
		m_loadedAssets.clear();

	}

	bool AssetLoader::loadAsset(const std::string& file,
								std::vector<Material*>& outMaterials,
								std::vector<Mesh*>& outMeshes,
								std::vector<Instance*>& outInstances) {

		// Check if asset is already loaded
		size_t hashed = strHash(file);
		for(size_t i = 0; i < m_loadedAssets.size(); ++i){
			if(m_loadedAssets[i].file == hashed){
				outMaterials = m_loadedAssets[i].materials;
				outMeshes = m_loadedAssets[i].meshes;
				outInstances = m_loadedAssets[i].instances;
				return true;
			}
		}

		// Assimp importer instance
		static Assimp::Importer importer;
		importer.FreeScene();

		// Split meshes at unsigned short's greatest value, so indexing with
		// unsinged short indices is assured to work
		importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT,
									USHRT_MAX);

		// Load and process the asset
		const aiScene* scene = importer.ReadFile(	file.c_str(),
													aiProcess_CalcTangentSpace | aiProcess_Triangulate
													| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
													| aiProcess_LimitBoneWeights | aiProcess_GenSmoothNormals
													| aiProcess_RemoveRedundantMaterials
													| aiProcess_GenUVCoords | aiProcess_FindInstances
													| aiProcess_SplitLargeMeshes
													| aiProcess_FixInfacingNormals);

		// Check for success
		if(!scene){
			burnWarn(importer.GetErrorString());
			return false;
		}

		// Reset temporary data storage
		m_instances.clear();
		m_meshes.clear();
		m_materials.clear();

		// Extract the asset's data. The order is important! Keep it!
		extractMaterials(scene);    // Independant
		extractMeshes(scene);    // Meshes depend on materials
		// Scan through nodes (depend on meshes)
		extractNodes(	scene->mRootNode,
						NULL);

		// Store into loaded assets list
		Asset asset;
		asset.file = hashed;
		asset.materials = m_materials;
		asset.meshes = m_meshes;
		asset.instances = m_instances;
		m_loadedAssets.push_back(asset);

		std::cout << "Asset loaded: " << m_materials.size() << " Materials, " << m_meshes.size()
		<< " Meshes, " << m_instances.size() << " Instances.\n";

		// Output the loaded data
		outMaterials = m_materials;
		outMeshes = m_meshes;
		outInstances = m_instances;

		return true;
	}

	void AssetLoader::extractMaterials(const aiScene* assScene) {

		for(unsigned int i = 0; i < assScene->mNumMaterials; ++i){

			aiMaterial* assMat = assScene->mMaterials[i];
			Material* burnMat = new Material();

			TextureStack diffuseStack, normalStack;

			//Diffuse
			aiColor3D diffuseColor(0.f);
			assMat->Get(AI_MATKEY_COLOR_DIFFUSE,
						diffuseColor);
			diffuseStack.setBaseColor(Vector3f(	diffuseColor.r,
												diffuseColor.g,
												diffuseColor.b));

			for(int ch = 0; ch != 8; ++ch){
				aiString path;

				if(assMat->GetTexture(	aiTextureType_DIFFUSE,
										ch,
										&path) == AI_SUCCESS){

					Texture* t = new Texture();

					if(t->loadFromFile(path.data)){

						Uint32 uvIndex = 0;
						float blending = 1.f;

						aiTextureOp aiOp;
						bool isOpExplicit = false;

						//aiTextureMapMode aiMapMode;

						assMat->Get(AI_MATKEY_TEXBLEND_DIFFUSE(ch),
									blending);
						assMat->Get(AI_MATKEY_UVWSRC_DIFFUSE(ch),
									uvIndex);
						if(assMat->Get(	AI_MATKEY_TEXOP_DIFFUSE(ch),
										aiOp) == AI_SUCCESS)
							isOpExplicit = true;

						diffuseStack.setTexture(t,
												ch);
						diffuseStack.setUvIndex(uvIndex,
												ch);
						diffuseStack.setBlending(	blending,
													ch);
						TextureStack::Operator op;
						if(isOpExplicit){
							switch (aiOp) {
								case aiTextureOp_Subtract:
									op = TextureStack::SUBTRACT;
									break;
								case aiTextureOp_Divide:
									op = TextureStack::DIVIDE;
									break;
								case aiTextureOp_Multiply:
									op = TextureStack::MULTIPLY;
									break;
								case aiTextureOp_SignedAdd:
									op = TextureStack::SIGNED_ADD;
									break;
								case aiTextureOp_SmoothAdd:
									op = TextureStack::SMOOTH_ADD;
									break;
								default:
									op = TextureStack::ADD;
									break;
							}
						}else{
							op = TextureStack::OVERWRITE;
						}
						diffuseStack.setOperator(	op,
													ch);

						std::cout << "Diffuse TextureStack instance! File: " << path.data << ", UvIndex: "
						<< uvIndex << ", Blending: " << blending << ", Op: " << op << "\n";

					}
				}

				if(assMat->GetTexture(	aiTextureType_NORMALS,
										ch,
										&path) == AI_SUCCESS || assMat->GetTexture(	aiTextureType_HEIGHT,
																					ch,
																					&path) == AI_SUCCESS){

					Texture* t = new Texture();

					if(t->loadFromFile(path.data)){

						Uint32 uvIndex = 0;
						float blending = 1.f;

						aiTextureOp aiOp;
						bool isOpExplicit = false;

						//aiTextureMapMode aiMapMode;

						assMat->Get(AI_MATKEY_TEXBLEND_NORMALS(ch),
									blending);
						assMat->Get(AI_MATKEY_UVWSRC_NORMALS(ch),
									uvIndex);
						if(assMat->Get(	AI_MATKEY_TEXOP_NORMALS(ch),
										aiOp) == AI_SUCCESS)
							isOpExplicit = true;

						normalStack.setTexture(	t,
												ch);
						normalStack.setUvIndex(	uvIndex,
												ch);
						normalStack.setBlending(blending,
												ch);

						TextureStack::Operator op;
						if(isOpExplicit){
							switch (aiOp) {
								case aiTextureOp_Subtract:
									op = TextureStack::SUBTRACT;
									break;
								case aiTextureOp_Divide:
									op = TextureStack::DIVIDE;
									break;
								case aiTextureOp_Multiply:
									op = TextureStack::MULTIPLY;
									break;
								case aiTextureOp_SignedAdd:
									op = TextureStack::SIGNED_ADD;
									break;
								case aiTextureOp_SmoothAdd:
									op = TextureStack::SMOOTH_ADD;
									break;
								default:
									op = TextureStack::ADD;
									break;
							}
						}else{
							op = TextureStack::OVERWRITE;
						}
						normalStack.setOperator(op,
												ch);

						std::cout << "Normal TextureStack instance! File: " << path.data << ", UvIndex: "
						<< uvIndex << ", Blending: " << blending << ", Op: " << op << "\n";

					}
				}

			}

			burnMat->setTextureStack(	Material::DIFFUSE,
										diffuseStack);
			burnMat->setTextureStack(	Material::NORMAL,
										normalStack);

			//Shininess
			float shininess = 1.f;
			assMat->Get(AI_MATKEY_SHININESS,
						shininess);
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
			// Extract Vertex Data: (Indexed)
			std::vector<Vertex> vertices;    // All unique vertices
			for(unsigned int v = 0; v < assMesh->mNumVertices; ++v){
				Vertex vert;
				//Now fetch the vertex data
				aiVector3D pos = assMesh->mVertices[v];
				aiVector3D norm = assMesh->mNormals[v];

				for(int ch = 0; ch != 8; ++ch){
					aiVector3D uv;
					if(assMesh->HasTextureCoords(ch)){
						uv = assMesh->mTextureCoords[ch][v];
					}
					vert.setUv(	Vector2f(	uv.x,
											uv.y),
								ch);
				}

				vert.setPosition(Vector3f(	pos.x,
											pos.y,
											pos.z));
				vert.setNormal(Vector3f(norm.x,
										norm.y,
										norm.z));

				if(assMesh->HasTangentsAndBitangents()){
					aiVector3D tangent = assMesh->mTangents[v];
					aiVector3D bitangent = assMesh->mBitangents[v];
					vert.setTangents(	Vector3f(	tangent.x,
													tangent.y,
													tangent.z),
										Vector3f(	bitangent.x,
													bitangent.y,
													bitangent.z));
				}

				vertices.push_back(vert);
			}
			burnMesh->addData(	&vertices[0],
								vertices.size());    // Store all vertices

			// Extract indices:
			std::vector<unsigned short> indices;
			for(unsigned int f = 0; f < assMesh->mNumFaces; ++f){
				const aiFace& face = assMesh->mFaces[f];

				for(unsigned int fv = 0; fv < face.mNumIndices; ++fv)
					indices.push_back(face.mIndices[fv]);

			}
			burnMesh->setIndices(indices);
			burnMesh->setRenderTechnique(Mesh::INDEXED);

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
			extractNodes(	node->mChildren[i],
							newInstance);

	}

} /* namespace burn */
