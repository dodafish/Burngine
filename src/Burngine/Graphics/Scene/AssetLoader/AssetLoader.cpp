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

#include <Burngine/Graphics/Scene/AssetLoader/AssetLoader.hpp>
#include <Burngine/System/Error.hpp>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

namespace burn {

bool AssetLoader::loadFromFile(const std::string& file, Model& target) {

	// Assimp importer instance
	Assimp::Importer importer;

	// Load and process the asset
	const aiScene* scene = importer.ReadFile(file.c_str(),
			aiProcess_CalcTangentSpace | aiProcess_Triangulate
					| aiProcess_JoinIdenticalVertices | aiProcess_SortByPType
					| aiProcess_LimitBoneWeights);

	// Check for success
	if (!scene) {
		burnWarn(importer.GetErrorString());
		return false;
	}

	// Extract the asset's data
	processScene(scene, target);

	return true;
}

void AssetLoader::processScene(const aiScene* scene, Model& target) {

	//TODO Process scene; after adaption of burngine structure

}

}
