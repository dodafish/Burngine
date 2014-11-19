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

#ifndef ASSETLOADER_HPP_
#define ASSETLOADER_HPP_

#include <Burngine/Graphics/Scene/Model.hpp>
#include <string>

#include <assimp/scene.h>

namespace burn {

/**
 * @brief Utitility class for generating Burngine Models by using Assimp
 */
class AssetLoader {
public:

	/**
	 * @brief Load a model from file.
	 *
	 * @param file Modelfile to load from
	 * @param target Model into which the data will be stored
	 *
	 *
	 * @return True on success. False otherwise
	 */
	static bool loadFromFile(const std::string& file, Model& target);

private:

	static void processScene(const aiScene* scene, Model& target);

};

} /* namespace burn */

#endif /* ASSETLOADER_HPP_ */
