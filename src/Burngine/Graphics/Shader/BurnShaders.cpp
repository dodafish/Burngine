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
#include <Burngine/Graphics/Shader/Shader.hpp>
#include <Burngine/System/Error.hpp>
#include <Burngine/System/Mutex.hpp>
#include <Burngine/System/Lock.hpp>
#include <fstream>

namespace {
/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

	burn::Mutex mutex;

	bool areInternalShadersLoaded = false;
	const std::string COLOR_SHADER_NAME = "COLOR";

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////
} /* namespace */

namespace burn {

	std::map<BurnShaders::Type, Shader*> BurnShaders::m_shaders;

	const Shader& BurnShaders::getShader(const Type& type) {

		if(!areInternalShadersLoaded){
			loadInternalShaders();
		}

		return *(m_shaders[type]);
	}

	void BurnShaders::loadInternalShaders() {

		Lock lock(mutex);

		if(areInternalShadersLoaded)
			return;

		// Open "burnshaders"
		std::ifstream burnshaders;
		burnshaders.open("./burnshaders");
		if(!burnshaders.is_open()){
			burnErr("Failed to load shaders! Cannot open 'burnshaders'");
		}

		// Get each line
		// Format: <TYPE> <VERTEX_FILE> <FRAGMENT_FILE>
		// Seperator: Whitespace/Newline
		std::string type;
		while(burnshaders >> type){

			// Next two strings are vertex and fragment shader
			std::string vertex, fragment;
			burnshaders >> vertex;
			burnshaders >> fragment;

			// Try loading the shader code
			Shader* shader = (new Shader(vertex, fragment));

			// Save shader
			if(type == COLOR_SHADER_NAME){
				m_shaders[COLOR] = shader;
			}else{
				burnshaders.close();
				burnErr("Failed loading shaders! Shader type unknown.");
			}

		}
		burnshaders.close();

		// Check
		for(int i = 0; i < COUNT; ++i){
			if(m_shaders.find(Type(COLOR + i)) == m_shaders.end()){
				burnErr("Failed loading shaders! Not all types were loaded!");
			}
		}

		areInternalShadersLoaded = true;
	}

	void BurnShaders::releaseInternalShaders() {

		Lock lock(mutex);

		if(!areInternalShadersLoaded)
			return;

		for(std::map<Type, Shader*>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
			delete it->second;
		m_shaders.clear();

		areInternalShadersLoaded = false;
	}

} /* namespace burn */