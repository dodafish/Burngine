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

#ifndef SRC_BURNGINE_GRAPHICS_SCENE_ASSETLOADER_HPP_
#define SRC_BURNGINE_GRAPHICS_SCENE_ASSETLOADER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Material.hpp>
#include <Burngine/Graphics/Scene/Mesh.hpp>
#include <Burngine/Graphics/Scene/Model.hpp>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace burn {

	class BURNGINE_API_EXPORT AssetLoader {
	public:

		static bool loadAsset(	const std::string& file,
								std::vector<Material*>& outMaterials,
								std::vector<Mesh*>& outMeshes,
								std::vector<Instance*>& outInstances);

		static void cleanup();

	private:

		static void extractMaterials(const aiScene* assScene);

		static void extractMeshes(const aiScene* assScene);

		/**
		 * @brief Recursive method scanning through assimp's node tree
		 */
		static void extractNodes(	aiNode* node,
									Instance* targetParent);

	private:

		struct Asset {
			size_t file;	///< Hashed file string
			std::vector<Material*> materials;
			std::vector<Mesh*> meshes;
			std::vector<Instance*> instances;
		};

		static std::vector<Asset> m_loadedAssets;

		static std::vector<Material*> m_materials;
		static std::vector<Mesh*> m_meshes;
		static std::vector<Instance*> m_instances;    ///< Node tree
	};

} /* namespace burn */

#endif /* SRC_BURNGINE_GRAPHICS_SCENE_ASSETLOADER_HPP_ */
