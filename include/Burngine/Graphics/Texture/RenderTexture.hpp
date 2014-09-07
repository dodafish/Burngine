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

#ifndef RENDERTEXTURE_HPP_
#define RENDERTEXTURE_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/Graphics/RenderTarget.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <Burngine/System/Math.hpp>

namespace burn {

	class BURNGINE_API_EXPORT RenderTexture : public RenderTarget {
	public:

		bool create(const Vector2ui& dimensions);

		void clear();

		const Texture& getTexture() const;

		virtual bool prepare() const;

		virtual Matrix4f getOrtho() const;

	private:
		GLuint m_framebuffer;    // Container for texture and depthbuffer
		GLuint m_depthbuffer;
		Texture m_texture;
		Vector2ui m_dimensions;
	};

} /* namespace burn */

#endif /* RENDERTEXTURE_HPP_ */
