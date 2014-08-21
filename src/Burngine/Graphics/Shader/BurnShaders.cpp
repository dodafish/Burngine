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

#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <fstream>

namespace {

bool areInternalShadersLoaded = false;

const std::string COLOR_SHADER_NAME = "COLOR";

}

namespace burn {

const Shader& BurnShaders::getShader(const Type& type) {

	if(!areInternalShadersLoaded){
		loadInternalShaders();
		areInternalShadersLoaded = true;
	}

	return m_shaders[type];
}

bool BurnShaders::loadInternalShaders() {

	// Open "burnshaders"
	std::ifstream burnshaders;
	burnshaders.open("./burnshaders");
	if(!burnshaders.is_open())
		return false;

	// Get each line
	// Format: <TYPE> <VERTEX_FILE> <FRAGMENT_FILE>\n
	std::string type;
	while(burnshaders >> type){

		// Next two strings are vertex and fragment shader
		std::string vertex, fragment;
		burnshaders >> vertex;
		burnshaders >> fragment;

		// Try loading the shader code
		Shader shader;
		if(!shader.load(vertex, fragment))
			return false;

		// Save shader
		switch (type) {
			case COLOR_SHADER_NAME:
				m_shaders[COLOR] = shader;
				break;
			default:
				std::cerr << "Failed loading shaders! Shader type unknown.\n";
				return false;
				break;
		}

	}

	// Check
	for(Type type = COLOR; type < COUNT; ++type){
		if(m_shaders.find(type) == m_shaders.end()){
			std::cerr << "Failed loading shaders! Not all types were loaded!\n";
			return false;
		}
	}

	return true;

}

} /* namespace burn */
