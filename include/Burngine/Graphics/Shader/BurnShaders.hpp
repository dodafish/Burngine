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
#include <map>
#include <string>

namespace burn {

	class Shader;

	/**
	 * @brief Handles internal shaders. They are automatically loaded
	 * with the first GlEntity or a call to getShader(), as they load
	 * together with the OpenGL initialization.
	 */
	class BURNGINE_API_EXPORT BurnShaders {
	public:

		/**
		 * @brief Shader types
		 */
		enum Type {
			COLOR = 0,    ///< Renders with a single 4-comp.-color; Keep first!
			TEXTURE,    ///< Renders with a single texture (optionally blending with a color)
			POINT_LIGHT,    ///< Rendering of point light's lighting
			DIRECTIONAL_LIGHT,    ///< Rendering of directional light's lighting
			SPOT_LIGHT,    ///< Rendering of spot light's lighting
			VSM,    ///< Rendering data into a VSM
			BLUR,    ///< PostEffect Blur on Textures
			GLOW,    ///< Extracts pixels according to their brightness
			FINALIZE,	///< Finalizes the deferred shading
			FONT2D,	///< For rendering 2D fonts
			BILLBOARD,	///< For rendering 2D fonts
			SKYBOX,	///< For rendering skyboxes
			SPRITE2D,	///< For rendering 2D sprites (GUI)
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

		/**
		 * @brief Load all Burngine shaders
		 */
		static void loadInternalShaders();

		/**
		 * @brief Unload all Burngine shaders
		 */
		static void releaseInternalShaders();

		/**
		 * @brief Set a custom path to the burnshaders file
		 */
		static void setBurnshadersPath(const std::string& path);

	private:

		static std::map<Type, Shader*> m_shaders;
		static std::string m_burnshadersPath;

	};

} /* namespace burn */

#endif /* BURNSHADERS_HPP_ */
