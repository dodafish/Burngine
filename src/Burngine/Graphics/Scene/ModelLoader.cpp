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

#include <Burngine/Graphics/Scene/ModelLoader.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/System/Math.hpp>
#include <Burngine/System/StringNumbers.hpp>
#include <Burngine/Graphics/Scene/AssetLoaders/ObjLoader.hpp>
#include <fstream>
#include <sstream>
#include <iostream>

namespace burn {
	namespace priv {

		std::map<size_t, Model> ModelLoader::m_models;

		bool ModelLoader::loadFromFile(	const std::string& fileName,
										Model& target) {

			// Check file name
			if(fileName.size() < 5){
				burnWarn("Cannot load model '" + fileName
				+ "'! String too short.");
				return false;
			}

			// Check file type
			FileType type = checkFileType(fileName);
			if(type == UNKNOWN){
				burnWarn("Cannot load model '" + fileName
				+ "'! Model type not supported.");
				return false;
			}

			// Correct the path to have a slash
			std::string fileNameFull = fileName;
			while(fileNameFull.find("\\") != std::string::npos){
				fileNameFull[fileNameFull.find("\\")] = '/';
			}
			if(fileNameFull.find("/") == std::string::npos){
				fileNameFull = "./" + fileNameFull;
				burnWarn("Corrected \"" + fileName + "\" to \"" + fileNameFull
				+ "\"");
			}

			// Try loading the model
			bool result = false;
			if(type == OBJ){
				ObjLoader objLoader;
				result = objLoader.load(fileNameFull, target);
			}

			if(!result){
				burnWarn("Failed to load model '" + fileNameFull + "'.");
				return false;
			}

			return true;
		}

		ModelLoader::FileType ModelLoader::checkFileType(const std::string& fileName) {

			FileType type = UNKNOWN;

			std::string endRaw = fileName.substr(fileName.size() - 3, 3);
			std::string end = "";

			// Convert all chars to upper case
			for(int i = 0; i != 3; ++i)
				end += std::toupper(endRaw[i]);

			if(end == "OBJ")
				type = OBJ;

			return type;
		}

	} /* namespace priv */
} /* namespace burn */
