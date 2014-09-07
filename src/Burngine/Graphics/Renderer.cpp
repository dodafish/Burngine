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

#include <Burngine/Graphics/Renderer.hpp>
#include <Burngine/Window/Window.hpp>
#include <Burngine/Graphics/Gui/GuiNode.hpp>
#include <Burngine/Graphics/Scene/SceneNode.hpp>
#include <Burngine/Graphics/Scene/Camera.hpp>
#include <Burngine/System/Math.hpp>

namespace burn {

	void Renderer::renderGuiNode(	const GuiNode& node,
									const RenderTarget& target) {

		if(target.prepare()){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

			node.render(Matrix4f(1.f), target.getOrtho());
		}
	}

	void Renderer::renderSceneNode(	const SceneNode& node,
									const RenderTarget& target,
									const Camera& camera) {

		if(target.prepare()){
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			glEnable(GL_DEPTH_TEST);
			glDepthFunc(GL_LESS);

			node.render(glm::lookAt(camera.getPosition(),
									Vector3f(0.f, 0.f, -0.1f),
									Vector3f(0.f, 1.f, 0.f)),
						glm::perspective<float>(30.f,
												16.f / 9.f,
												0.01f,
												10000.f));
		}

	}

} /* namespace burn */
