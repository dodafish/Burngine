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

#ifndef SHADER_HPP_
#define SHADER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <string>

namespace burn {

	/**
	 * @brief Can load one internal shadertype and handle its parameters
	 */
	class BURNGINE_API_EXPORT Shader : public GlEntity {

	public:

		/**
		 * @brief Load code of specific type
		 *
		 * @param vertex Vertex shader
		 * @param fragment Fragment shader
		 *
		 * @return True on success
		 */
		Shader(	const std::string& vertex,
				const std::string& fragment);

		~Shader();

		/**
		 * @brief Activate shader for current context
		 */
		void activate() const;

	private:

		/**
		 * @brief Releases used memory
		 */
		void cleanup();

	private:

		GLuint m_id;    ///< Shader ID

	};

} /* namespace burn */

#endif /* SHADER_HPP_ */