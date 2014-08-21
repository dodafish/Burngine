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

#ifndef BURNSHADERS_HPP_
#define BURNSHADERS_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Shader/Shader.hpp>
#include <Burngine/System/NonInstancable.hpp>
#include <map>
#include <memory>

namespace burn {

/**
 * @brief Can load all Burngine shaders and handle these
 */
class BURNGINE_API_EXPORT BurnShaders : public NonInstancable {
public:

	/**
	 * @brief Shader types
	 */
	enum Type {
		COLOR = 0,    ///< Renders with a single 4-comp.-color; Keep first!
		COUNT    ///< Keep last!
	};

public:

	/**
	 * @brief Get a Burngine shader
	 *
	 * @param type Required Shadertype
	 *
	 * @return shader of given type
	 */
	static const Shader& getShader(const Type& type);

private:

	/**
	 * @brief Load all Burngine shaders
	 */
	static bool loadInternalShaders();

private:

	static std::map<Type, std::shared_ptr<Shader>> m_shaders;

};

} /* namespace burn */

#endif /* BURNSHADERS_HPP_ */
