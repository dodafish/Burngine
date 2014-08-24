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

#include <Burngine/Graphics/Shader/Shader.hpp>
#include <Burngine/Window/GlContext.hpp>
#include <Burngine/System/Error.hpp>

#include <vector>
#include <fstream>
#include <iostream>
#include <streambuf>

namespace burn {
	namespace priv {

		namespace {

			bool areInternalShadersLoaded = false;

			const std::string COLOR_SHADER_NAME = "COLOR";

		}

		std::map<Shader::Type, Shader*> Shader::m_shaders;

		const Shader& Shader::getShader(const Type& type) {

			if(!areInternalShadersLoaded){
				burnErr("Unable to get shader. OpenGL was not initialized!");
			}

			return *(m_shaders[type]);
		}

		void Shader::loadInternalShaders() {

			if(areInternalShadersLoaded)
				return;

			// Open "burnshaders"
			std::ifstream burnshaders;
			burnshaders.open("./burnshaders");
			if(!burnshaders.is_open()){
				burnErr("Failed to load shaders! Cannot open 'burnshaders'");
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
				Shader* shader = new Shader(vertex, fragment);

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

		void Shader::releaseInternalShaders() {

			if(!areInternalShadersLoaded)
				return;

			for(std::map<Shader::Type, Shader*>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it){
				delete it->second;
			}
			m_shaders.clear();

			areInternalShadersLoaded = false;
		}

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

		Shader::Shader(	const std::string& vertex,
						const std::string& fragment) :
		m_id(0) {

			GlContext::ensureContext();

			std::cout << "Loading shader: " << vertex << " " << fragment << " ...\n";

			// Create the shaders
			GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
			GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

			// Read the Vertex Shader code from the file
			std::ifstream vertexFileStream(vertex);
			std::string vertexShaderCode;
			vertexFileStream.seekg(0, std::ios::end);
			vertexShaderCode.reserve(vertexFileStream.tellg());
			vertexFileStream.seekg(0, std::ios::beg);
			vertexShaderCode.assign((std::istreambuf_iterator<char>(vertexFileStream)),
									std::istreambuf_iterator<char>());
			//std::cout << vertexShaderCode << "\n";

			// Read the Fragment Shader code from the file
			std::ifstream fragmentFileStream(fragment);
			std::string fragmentShaderCode;
			fragmentFileStream.seekg(0, std::ios::end);
			fragmentShaderCode.reserve(fragmentFileStream.tellg());
			fragmentFileStream.seekg(0, std::ios::beg);
			fragmentShaderCode.assign(	(std::istreambuf_iterator<char>(fragmentFileStream)),
										std::istreambuf_iterator<char>());
			//std::cout << fragmentShaderCode << "\n";

			GLint Result = GL_FALSE;
			int InfoLogLength;

			// Compile Vertex Shader
			char const * VertexSourcePointer = vertexShaderCode.c_str();
			glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
			glCompileShader(VertexShaderID);

			// Check Vertex Shader
			glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> VertexShaderErrorMessage(InfoLogLength);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			if(!Result){
				std::cerr << "Failed compiling vertex shader!\n";
				std::string err;
				for(size_t i = 0; i < VertexShaderErrorMessage.size(); ++i)
					err += VertexShaderErrorMessage[i];
				burnErr(err);
			}

			// Compile Fragment Shader
			char const * FragmentSourcePointer = fragmentShaderCode.c_str();
			glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
			glCompileShader(FragmentShaderID);

			// Check Fragment Shader
			glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
			glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			if(!Result){
				std::cerr << "Failed compiling fragment shader!\n";
				std::string err;
				for(size_t i = 0; i < FragmentShaderErrorMessage.size(); ++i)
					err += FragmentShaderErrorMessage[i];
				burnErr(err);
			}

			// Link the program
			GLuint ProgramID = glCreateProgram();
			glAttachShader(ProgramID, VertexShaderID);
			glAttachShader(ProgramID, FragmentShaderID);
			glLinkProgram(ProgramID);

			// Check the program
			glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
			glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
			std::vector<char> ProgramErrorMessage(std::max(InfoLogLength, int(1)));
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			if(!Result){
				std::cerr << "Failed linking shaders or some other error!\n";
				std::string err;
				for(size_t i = 0; i < ProgramErrorMessage.size(); ++i)
					err += ProgramErrorMessage[i];
				burnErr(err);
			}

			glDeleteShader(VertexShaderID);
			glDeleteShader(FragmentShaderID);

			fragmentFileStream.close();
			vertexFileStream.close();

			m_id = ProgramID;

		}

		Shader::~Shader() {
			std::cout << "Destroying shader...\n";
			cleanup();

		}

		void Shader::cleanup() {

			if(m_id != 0){
				GlContext::ensureContext();
				glDeleteProgram(m_id);
			}

			m_id = 0;

		}

		void Shader::activate() const {
			//glUseProgram(m_id);
		}

	} /* namespace priv */
} /* namespace burn */
