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

#ifndef RENDERER_HPP_
#define RENDERER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/Graphics/Texture/RenderTexture.hpp>

namespace burn {

	class RenderTarget;
	class GuiNode;
	class SceneNode;
	class Camera;

	/**
	 * @brief Renders Renderables into a RenderTarget
	 */
	class BURNGINE_API_EXPORT Renderer : public GlEntity {
	public:

		/**
		 * @brief Creates render textures
		 */
		Renderer();

		/**
		 * @brief Prepare for rendering. Adjust render textures
		 *
		 * @param targetDimensions dimensions of the final render target
		 */
		void prepare(const Vector2ui& targetDimensions);

		/**
		 * @brief Put the render result into target
		 *
		 * @param target final render target
		 */
		void finalize(const RenderTarget& target);

		/**
		 * @brief Render a GuiNode
		 */
		void renderGuiNode(const GuiNode& node);

		/**
		 * @brief Render a SceneNode
		 */
		void renderSceneNode(	const SceneNode& node,
								const Camera& camera);

	private:
		RenderTexture m_diffuseRenderTexture;
	};

} /* namespace burn */

#endif /* RENDERER_HPP_ */
