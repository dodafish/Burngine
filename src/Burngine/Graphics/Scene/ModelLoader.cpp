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
#include <fstream>

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

			// Try loading the model
			bool result = false;
			if(type == OBJ)
				result = loadObj(fileName, target);

			if(!result){
				burnWarn("Failed to load model '" + fileName + "'.");
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

		bool ModelLoader::loadObj(const std::string& fileName, Model& target){

			// Try opening the file
			std::fstream file(fileName, std::ios::in);
			if(!file.is_open()){
				burnWarn("Cannot load OBJ '" + fileName + "'! Unable to open file.");
				return false;
			}



			file.close();
			return true;
		}

	} /* namespace priv */
} /* namespace burn */
