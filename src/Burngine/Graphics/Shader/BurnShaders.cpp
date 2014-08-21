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
#include <iostream>

namespace {

bool areInternalShadersLoaded = false;

const std::string COLOR_SHADER_NAME = "COLOR";

}

namespace burn {

std::map<BurnShaders::Type, std::shared_ptr<Shader>> BurnShaders::m_shaders;

const Shader& BurnShaders::getShader(const Type& type) {

	if(!areInternalShadersLoaded){
		if(!loadInternalShaders()){
			std::cerr << "Failed to load shaders!\n";
			static Shader defaultShader;
			return defaultShader;
		}
		areInternalShadersLoaded = true;
	}

	return *(m_shaders[type].get());
}

bool BurnShaders::loadInternalShaders() {

	// Open "burnshaders"
	std::ifstream burnshaders;
	burnshaders.open("./burnshaders");
	if(!burnshaders.is_open()){
		std::cerr << "Failed to load shaders! Cannot open 'burnshaders'.\n";
		return false;
	}

	// Get each line
	// Format: <TYPE> <VERTEX_FILE> <FRAGMENT_FILE>\n
	std::string type;
	while(burnshaders >> type){

		// Next two strings are vertex and fragment shader
		std::string vertex, fragment;
		burnshaders >> vertex;
		burnshaders >> fragment;

		// Try loading the shader code
		std::shared_ptr<Shader> shader(new Shader());
		if(!shader->load(vertex, fragment)){
			std::cerr << "Failed loading shader!\n";
			burnshaders.close();
			return false;
		}

		// Save shader
		if(type == COLOR_SHADER_NAME){
			m_shaders[COLOR] = shader;
		}else{
			std::cerr << "Failed loading shaders! Shader type unknown.\n";
			burnshaders.close();
			return false;
		}

	}
	burnshaders.close();

	// Check
	for(int i = 0; i < COUNT; ++i){
		if(m_shaders.find(Type(COLOR + i)) == m_shaders.end()){
			std::cerr << "Failed loading shaders! Not all types were loaded!\n";
			return false;
		}
	}

	return true;

}

} /* namespace burn */
