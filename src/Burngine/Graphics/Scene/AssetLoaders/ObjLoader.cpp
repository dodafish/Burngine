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

			return s.size() != 0;
		}

		bool ObjLoader::parseObjectLine(const std::string& line) {

			if(line[0] == '#' || line[0] == '\n'){
				// Comment line or new line!
				return true;
			}else if(line[0] != ' '){
				// Line has data -> must have minimum size of 2
				// Avoid out-of-range exceptions
				if(line.size() < 2){
					burnWarn("Failed to parse vertex data.");
					return false;
				}
			}

			if(line[0] == 'v'){
				// Some vertex data!

				// If no mesh was added
				if(m_meshData.size() == 0)
					m_meshData.push_back(MeshData());

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
					m_meshData.back().uvs.push_back(uv);
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
					m_meshData.back().normals.push_back(normal);
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
					m_meshData.back().positions.push_back(position);
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

				std::string mtllibPath = m_objectFileName.substr(0, lastSlash)
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

		bool ObjLoader::loadMtllib(const std::string&) {

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
					>= m_meshData[i].positions.size()){
						burnWarn("Index is out of range.");
						return false;
					}
					v.setPosition(m_meshData[i].positions[m_meshData[i].indices[j]
					- 1]);

					// Vertex UV
					if(m_meshData[i].componentCount == 3){
						if(++j >= m_meshData[i].indices.size()){
							burnWarn("Index is out of range.");
							return false;
						}

						if(static_cast<size_t>(m_meshData[i].indices[j] - 1)
						>= m_meshData[i].uvs.size()){
							burnWarn("Index is out of range.");
							return false;
						}
						v.setUv(m_meshData[i].uvs[m_meshData[i].indices[j] - 1]);
					}

					// Vertex Normal
					if(m_meshData[i].componentCount >= 2){
						if(++j >= m_meshData[i].indices.size()){
							burnWarn("Index is out of range.");
							return false;
						}

						if(static_cast<size_t>(m_meshData[i].indices[j] - 1)
						>= m_meshData[i].normals.size()){
							burnWarn("Index is out of range.");
							return false;
						}
						v.setNormal(m_meshData[i].normals[m_meshData[i].indices[j]
						- 1]);
					}

					// Save vertex
					vertices.push_back(v);
				}

				// Create mesh
				Mesh mesh;
				mesh.loadFromData(&vertices[0], vertices.size());

				// Add to model
				m_model.addMesh(mesh);

			}

			return true;
		}

	} /* namespace priv */
} /* namespace burn */
