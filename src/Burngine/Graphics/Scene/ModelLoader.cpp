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
			if(type == OBJ)
				result = loadObj(fileNameFull, target);

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

		bool ModelLoader::loadObj(	const std::string& fileName,
									Model& target) {

			// Try opening the file
			std::fstream file(fileName, std::ios::in);
			if(!file.is_open()){
				burnWarn("Cannot load OBJ '" + fileName
				+ "'! Unable to open file.");
				return false;
			}

			// Meshes:
			std::vector<Mesh> meshes;
			// Vertex data
			std::vector<Vector3f> positions, normals;
			std::vector<Vector2f> uvs;
			std::vector<Vertex> vertices;

			// Read each line
			std::string line;
			StringNumbers sn;    // For extracting numbers
			while(std::getline(file, line)){

				// Check the line header
				if(line.find("#") != std::string::npos){
					// Comment line
					continue;
				}else if(line.find("o ") != std::string::npos){

					if(vertices.size() != 0){
						if(meshes.size() == 0)
							meshes.push_back(Mesh());
						meshes.back().loadFromData(	&vertices[0],
													vertices.size());
					}

					// New mesh!
					meshes.push_back(Mesh());
					positions.clear();
					normals.clear();
					vertices.clear();

				}else if(line.find("vt ") != std::string::npos){
					// Normal data!

					Vector2f uv;

					sn.setString(line);
					if(!sn.nextFloat(uv.x) || !sn.nextFloat(uv.y)){
						burnWarn("Failed to load OBJ! Unable to parse line.");
						return false;
					}

					uvs.push_back(uv);

				}else if(line.find("vn ") != std::string::npos){
					// Normal data!

					Vector3f normal;

					sn.setString(line);
					if(!sn.nextFloat(normal.x) || !sn.nextFloat(normal.y)
					|| !sn.nextFloat(normal.z)){
						burnWarn("Failed to load OBJ! Unable to parse line.");
						return false;
					}

					normals.push_back(normal);

				}else if(line.find("v ") != std::string::npos){
					// Position data!

					Vector3f pos;

					sn.setString(line);
					if(!sn.nextFloat(pos.x) || !sn.nextFloat(pos.y)
					|| !sn.nextFloat(pos.z)){
						burnWarn("Failed to load OBJ! Unable to parse line.");
						return false;
					}

					positions.push_back(pos);

				}else if(line.find("f ") != std::string::npos){

					int numComponents = 0;

					if(line.find("//") != std::string::npos){
						numComponents = 2;
					}else if(line.find("/") != std::string::npos){
						numComponents = 3;
					}else{
						numComponents = 1;
					}

					sn.setString(line);

					for(int i = 0; i != 3; ++i){
						Vertex vertex;
						int index = 1;

						// Get position data
						if(!sn.nextInt(index)){
							burnWarn("Failed to load OBJ! Unable to parse line.");
							return false;
						}
						if(static_cast<size_t>(index - 1)
						>= positions.size()){
							burnWarn("Failed to load OBJ! Index out of range.");
							return false;
						}
						vertex.setPosition(positions[index - 1]);

						// Is there texture data?
						if(numComponents == 3){
							if(!sn.nextInt(index)){
								burnWarn("Failed to load OBJ! Unable to parse line.");
								return false;
							}
							if(static_cast<size_t>(index - 1) >= uvs.size()){
								burnWarn("Failed to load OBJ! Index out of range.");
								return false;
							}
							vertex.setUv(uvs[index - 1]);
						}

						// Is there normal data?
						if(numComponents >= 2){
							if(!sn.nextInt(index)){
								burnWarn("Failed to load OBJ! Unable to parse line.");
								return false;
							}
							if(static_cast<size_t>(index - 1)
							>= normals.size()){
								burnWarn("Failed to load OBJ! Index out of range.");
								return false;
							}
							vertex.setNormal(normals[index - 1]);
						}

						vertices.push_back(vertex);
					}

					// Are there even more indices in this line?
					// Maybe: Too many components?
					int dummy = 0;
					if(sn.nextInt(dummy)){
						burnWarn("Failed to load OBJ! Too many indices.");
						return false;
					}

				}

			}

			if(vertices.size() != 0){
				if(meshes.size() == 0)
					meshes.push_back(Mesh());
				meshes.back().loadFromData(&vertices[0], vertices.size());
			}

			target.clear();
			for(size_t i = 0; i < meshes.size(); ++i)
				target.addMesh(meshes[i]);

			file.close();
			return true;
		}

	} /* namespace priv */
} /* namespace burn */
