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

#include <Burngine/Graphics/Scene/AssetLoaders/ObjLoader.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/System/StringNumbers.hpp>
#include <Burngine/Graphics/Vertex.hpp>

namespace {

	void crop(std::string& s) {
		// Remove leading whitespaces
		while(s.size() > 0 && s[0] == ' ')
			s.erase(s.begin());
		// Cap at end of name
		size_t last = s.find(" ");
		if(last != std::string::npos)
			s = s.substr(0, last);
	}

}

namespace burn {
	namespace priv {

		ObjLoader::MeshData::MeshData() :
		componentCount(0) {

		}

		ObjLoader::MaterialData::MaterialData() :
		diffuseColor(0.75f) {

		}

		bool ObjLoader::load(	const std::string& fileName,
								Model& target) {

			// Delete cache
			reset();

			// Load cache
			if(!loadIntoCache(fileName))
				return false;

			// Process cache
			if(!processCache())
				return false;

			// Assign model
			target = m_model;

			return true;
		}

		void ObjLoader::reset() {
			m_model.clear();
			m_meshData.clear();
			m_positions.clear();
			m_normals.clear();
			m_uvs.clear();
			m_materialData.clear();
		}

		bool ObjLoader::loadIntoCache(const std::string& fileName) {

			// Try opening the file
			m_objectFile.open(fileName, std::ios::in);
			if(!m_objectFile.is_open()){
				burnWarn("Cannot load OBJ '" + fileName
				+ "'! Unable to open object file.");
				return false;
			}
			m_objectFileName = fileName;

			// Read
			std::string line;
			while(nextLine(m_objectFile, line))
				if(!parseObjectLine(line))
					return false;

			// Close file
			m_objectFile.close();

			return true;
		}

		bool ObjLoader::nextLine(	std::fstream& f,
									std::string& s) {

			if(!f.is_open())
				return false;

			if(!std::getline(f, s))
				return false;

			// Remove leading white spaces
			while(s.size() > 0 && s[0] == ' '){
				s.erase(s.begin());
			}

			return true;
		}

		bool ObjLoader::parseObjectLine(const std::string& line) {

			//Skip empty lines
			if(line.size() == 0)
				return true;

			if(line[0] == '#' || line[0] == '\n'){
				// Comment line or new line!
				return true;
			}

			// Line must have data at this point
			if(line.size() < 2){
				burnWarn("Line with data is too short");
				return false;
			}

			if(line[0] == 'o'){
				// New mesh!
				m_meshData.push_back(MeshData());
			}else if(line[0] == 'v'){
				// Some vertex data!

				// Identify data type
				if(line[1] == 't'){
					// Texture coordinate!
					Vector2f uv;

					StringNumbers sn;
					sn.setString(line);

					if(!sn.nextFloat(uv.x)){
						burnWarn("Failed to read texture coords.");
						return false;
					}
					if(!sn.nextFloat(uv.y)){
						burnWarn("Failed to read texture coords.");
						return false;
					}

					// Save
					m_uvs.push_back(uv);
				}else if(line[1] == 'n'){
					// Normal!
					Vector3f normal;

					StringNumbers sn;
					sn.setString(line);

					if(!sn.nextFloat(normal.x)){
						burnWarn("Failed to read normal.");
						return false;
					}
					if(!sn.nextFloat(normal.y)){
						burnWarn("Failed to read normal.");
						return false;
					}
					if(!sn.nextFloat(normal.z)){
						burnWarn("Failed to read normal.");
						return false;
					}

					// Save
					m_normals.push_back(normal);
				}else if(line[1] == ' '){
					// Position!
					Vector3f position;

					StringNumbers sn;
					sn.setString(line);

					if(!sn.nextFloat(position.x)){
						burnWarn("Failed to read normal.");
						return false;
					}
					if(!sn.nextFloat(position.y)){
						burnWarn("Failed to read normal.");
						return false;
					}
					if(!sn.nextFloat(position.z)){
						burnWarn("Failed to read normal.");
						return false;
					}

					// Save
					m_positions.push_back(position);
				}else{
					// Unkown vertex data line!
					burnWarn("Failed to parse unknown vertex data line.");
					return false;
				}

			}else if(line[0] == 'f'){
				// Indices!

				// If no mesh was added
				if(m_meshData.size() == 0)
					m_meshData.push_back(MeshData());

				// Estimate number of components
				if(m_meshData.back().componentCount == 0){
					if(line.find("//") != std::string::npos)
						m_meshData.back().componentCount = 2;    // Pos//Normal
					else if(line.find("/") != std::string::npos)
						m_meshData.back().componentCount = 3;    // Pos/UV/Normal
					else
						m_meshData.back().componentCount = 1;    // Pos
				}

				// Read indices
				StringNumbers sn;
				sn.setString(line);
				int index = 1;
				while(sn.nextInt(index)){
					m_meshData.back().indices.push_back(index);
				}

			}else if(line.size() > 6 && line.substr(0, 6) == "mtllib"){
				// Material library file!

				// Read the name
				std::string mtllibName = line.substr(6, line.size() - 6);

				// Format the string
				crop(mtllibName);

				// Check if its still valid
				// -> "*.mtl" = file type + dot + at least one character
				if(mtllibName.size() < 5){
					burnWarn("Cannot load mtllib. File name too short: \""
					+ mtllibName + "\"");
					return false;
				}

				// Construct the path for the mtllib
				size_t lastSlash = m_objectFileName.rfind("/");
				if(lastSlash == std::string::npos){
					burnWarn("Cannot load mtllib. Path is corrupt.");
					return false;
				}

				std::string mtllibPath = m_objectFileName.substr(	0,
																	lastSlash
																	+ 1)
				+ mtllibName;

				if(!loadMtllib(mtllibPath)){
					burnWarn("Failed to load mtllib.");
					return false;
				}

			}else if(line.size() > 6 && line.substr(0, 6) == "usemtl"){
				// Mesh material name!

				// If no mesh was added
				if(m_meshData.size() == 0)
					m_meshData.push_back(MeshData());

				// Read the name
				std::string materialName = line.substr(6, line.size() - 6);

				// Format the name
				crop(materialName);

				// Check if its still valid
				// -> "*.mtl" = file type + dot + at least one character
				if(materialName.size() == 0){
					burnWarn("Failed assigning material name.");
					return false;
				}

				// Save
				m_meshData.back().materialName = materialName;

			}else{
				burnWarn("Skipped unknown line with data: \"" + line + "\"");
			}

			return true;
		}

		bool ObjLoader::parseMaterialLine(const std::string& line) {

			//Skip empty lines
			if(line.size() == 0)
				return true;

			if(line[0] == '#' || line[0] == '\n'){
				// Comment line or new line!
				return true;
			}

			// Line must have data at this point
			if(line.size() < 2){
				burnWarn("Line with data is too short");
				return false;
			}

			if(line.size() > 6 && line.substr(0, 6) == "newmtl"){
				// New material!

				std::string name = line.substr(6, line.size() - 6);
				crop(name);

				if(name.size() == 0){
					burnWarn("Material name too short.");
					return false;
				}

				m_materialData.push_back(MaterialData());
				m_materialData.back().name = name;

			}else if(line[0] == 'K' && line[1] == 'd'){
				// Diffuse color!

				// Do we have a material?
				if(m_materialData.size() == 0){
					burnWarn("Cannot store data. No material was created.");
					return false;
				}

				Vector3f color;

				StringNumbers sn;
				sn.setString(line);

				// Read values
				if(!sn.nextFloat(color.r) || !sn.nextFloat(color.g)
				|| !sn.nextFloat(color.b)){
					burnWarn("Failed to read diffuse color.");
					return false;
				}

				m_materialData.back().diffuseColor = color;
			}else if(line.size() > 6 && line.substr(0, 6) == "map_Kd"){
				// Diffuse texture!

				// Do we have a material?
				if(m_materialData.size() == 0){
					burnWarn("Cannot store data. No material was created.");
					return false;
				}

				std::string path = line.substr(6, line.size() - 6);
				crop(path);

				if(path.size() == 0){
					burnWarn("Texture path too short.");
					return false;
				}

				m_materialData.back().diffuseTexturePath = path;
			}else{
				burnWarn("Skipped unknown line with data: \"" + line + "\"");
			}

			return true;
		}

		bool ObjLoader::loadMtllib(const std::string& fileName) {

			// Try opening the file
			std::fstream materialFile(fileName, std::ios::in);
			if(!materialFile.is_open()){
				burnWarn("Cannot open mtllib: " + fileName);
				return false;
			}

			// Read
			std::string line;
			while(nextLine(materialFile, line))
				if(!parseMaterialLine(line))
					return false;

			// Close file
			materialFile.close();

			return true;
		}

		bool ObjLoader::processCache() {

			// Check cache
			if(m_meshData.size() == 0){
				burnWarn("No mesh data loaded.");
				return false;
			}

			// Process each mesh
			for(size_t i = 0; i < m_meshData.size(); ++i){

				// Generate the vertices
				std::vector<Vertex> vertices;

				for(size_t j = 0; j < m_meshData[i].indices.size(); ++j){
					Vertex v;

					// Vertex position
					if(static_cast<size_t>(m_meshData[i].indices[j] - 1)
					>= m_positions.size()){
						burnWarn("Index is out of range.");
						return false;
					}
					v.setPosition(m_positions[m_meshData[i].indices[j] - 1]);

					// Vertex UV
					if(m_meshData[i].componentCount == 3){
						if(++j >= m_meshData[i].indices.size()){
							burnWarn("Index is out of range.");
							return false;
						}

						if(static_cast<size_t>(m_meshData[i].indices[j] - 1)
						>= m_uvs.size()){
							burnWarn("Index is out of range.");
							return false;
						}
						v.setUv(m_uvs[m_meshData[i].indices[j] - 1]);
					}

					// Vertex Normal
					if(m_meshData[i].componentCount >= 2){
						if(++j >= m_meshData[i].indices.size()){
							burnWarn("Index is out of range.");
							return false;
						}

						if(static_cast<size_t>(m_meshData[i].indices[j] - 1)
						>= m_normals.size()){
							burnWarn("Index is out of range.");
							return false;
						}
						v.setNormal(m_normals[m_meshData[i].indices[j] - 1]);
					}

					// Save vertex
					vertices.push_back(v);
				}

				// Create mesh
				Mesh mesh;
				mesh.loadFromData(&vertices[0], vertices.size());

				// Set material
				for(size_t j = 0; j < m_materialData.size(); ++j){
					if(m_materialData[j].name == m_meshData[i].materialName){
						Material mat;
						mat.setDiffuseColor(m_materialData[j].diffuseColor);

						if(m_materialData[j].diffuseTexturePath.size() != 0){
							// It has a texture!
							Texture texture;
							if(texture.loadFromFile(m_materialData[j].diffuseTexturePath)){
								mat.setDiffuseTexture(texture);
							}
						}

						mesh.setMaterial(mat);
						break;
					}
				}

				// Add to model
				m_model.addMesh(mesh);

			}

			return true;
		}

	} /* namespace priv */
} /* namespace burn */
