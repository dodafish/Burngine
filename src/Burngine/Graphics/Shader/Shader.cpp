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
#include <streambuf>
#include <fstream>
#include <vector>
#include <iostream>
#include <pthread.h>

namespace burn {

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

	Shader::Shader(	const std::string& vertex,
					const std::string& fragment) :
	m_id(0) {

		std::cout << "Shader creation in thread: " << pthread_self().p << "\n";

		ensureContext();

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
		vertexShaderCode.assign((std::istreambuf_iterator<char>(vertexFileStream)), std::istreambuf_iterator<char>());
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
		if(ProgramID == 0)
			burnErr("Failed to create program!");

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
		std::cout << "Shader deletion in thread: " << pthread_self().p << "\n";
		cleanup();
	}

	void Shader::cleanup() {

		/*if(m_id != 0){

			std::cout << "ensuring context...\n";
			ensureContext();

			glUseProgram(0);

			if(!glIsProgram(m_id))
				std::cerr << "This program does not exist!\n";
			else
				std::cout << "This program does really exist.\n";

			std::cout << "deleting program...\n";
			glDeleteProgram(m_id);

			if(glGetError() == GL_INVALID_VALUE)
				burnErr("Program ID is invalid (i.e. not created by OpenGL)");

			std::cout << "program deleted\n";

		}*/

		m_id = 0;

	}

	void Shader::activate() const {
		//glUseProgram(m_id);
	}

} /* namespace burn */
