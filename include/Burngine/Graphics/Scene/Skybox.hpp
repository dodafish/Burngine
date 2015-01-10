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

#ifndef INCLUDE_BURNGINE_GRAPHICS_SCENE_SKYBOX_HPP_
#define INCLUDE_BURNGINE_GRAPHICS_SCENE_SKYBOX_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Texture/CubeMap.hpp>
#include <Burngine/Graphics/VertexArray.hpp>
#include <Burngine/Graphics/VertexBuffer.hpp>
#include <Burngine/Window/GlEntity.hpp>

namespace burn {

	class BURNGINE_API_EXPORT Skybox : public GlEntity{
	public:

		Skybox();

		void setCubeMap(const CubeMap& cubeMap);

		const CubeMap& getCubeMap() const;

		void render(const Vector3f& cameraPosition, const Matrix4f& view, const Matrix4f& projection) const ;

	private:

		/**
		 * @brief Update vertex buffer data
		 */
		void updateVertexData();

		/**
		 * @brief Update vertex array object if necessary
		 */
		void ensureUpdatedVertexArray() const;

	private:
		CubeMap m_cubeMap;
		VertexArray m_vertexArray;
		VertexBuffer m_vertexBuffer;
	};

} /* namespace burn */

#endif /* INCLUDE_BURNGINE_GRAPHICS_SCENE_SKYBOX_HPP_ */
