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
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <streambuf>
#include <fstream>
#include <vector>
#include <iostream>

#ifndef max
#define max(a, b) (a > b) ? a : b
#endif

namespace burn {

/////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////

	Shader::Shader(	const std::string& vertex,
					const std::string& fragment) :
	m_id(0),
	m_textureUnitCounter(0) {

		ensureContext();

		std::cout << "Loading shader: " << vertex << " " << fragment << " ...\n";

		// Create the shaders
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		// Read the Vertex Shader code from the file
		std::ifstream vertexFileStream(vertex);
		std::string vertexShaderCode;
		vertexFileStream.seekg(0, std::ios::end);
		vertexShaderCode.reserve(static_cast<size_t>(vertexFileStream.tellg()));
		vertexFileStream.seekg(0, std::ios::beg);
		vertexShaderCode.assign((std::istreambuf_iterator<char>(vertexFileStream)), std::istreambuf_iterator<char>());
		//std::cout << vertexShaderCode << "\n";

		// Read the Fragment Shader code from the file
		std::ifstream fragmentFileStream(fragment);
		std::string fragmentShaderCode;
		fragmentFileStream.seekg(0, std::ios::end);
		fragmentShaderCode.reserve(static_cast<size_t>(fragmentFileStream.tellg()));
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
		std::vector<char> ProgramErrorMessage(max(InfoLogLength, int(1)));
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
		cleanup();
	}

	void Shader::cleanup() {

		if(m_id != 0){
			ensureContext();
			glDeleteProgram(m_id);
		}

		m_id = 0;

	}

	void Shader::activate() const {
		ensureContext();
		glUseProgram(m_id);
	}

	void Shader::resetTextureUnitCounter() const {
		m_textureUnitCounter = 0;
	}

	void Shader::bindTexture(	const std::string& samplerName,
								const Texture& texture) const {
		ensureContext();

		setUniform(samplerName, m_textureUnitCounter);
		glActiveTexture(GL_TEXTURE0 + m_textureUnitCounter);
		glBindTexture(GL_TEXTURE_2D, texture.getId());

		++m_textureUnitCounter;
	}

	void Shader::setUniform(const std::string& name,
							const Matrix4f& value) const {
		ensureContext();
		glUseProgram(m_id);
		glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &value[0][0]);
	}

	void Shader::setUniform(const std::string& name,
							const Vector4f& value) const {
		ensureContext();
		glUseProgram(m_id);
		glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
	}

	void Shader::setUniform(const std::string& name,
							const Vector3f& value) const {
		ensureContext();
		glUseProgram(m_id);
		glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
	}

	void Shader::setUniform(const std::string& name,
							const Vector2f& value) const {
		ensureContext();
		glUseProgram(m_id);
		glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
	}

	void Shader::setUniform(const std::string& name,
							const float& value) const {
		ensureContext();
		glUseProgram(m_id);
		glUniform1fv(glGetUniformLocation(m_id, name.c_str()), 1, &value);
	}

	void Shader::setUniform(const std::string& name,
							const int& value) const {
		ensureContext();
		glUseProgram(m_id);
		glUniform1iv(glGetUniformLocation(m_id, name.c_str()), 1, &value);
	}

} /* namespace burn */
