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

#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/Window/GlContext.hpp>
#include <Burngine/Graphics/Shader/BurnShaders.hpp>
#include <Burngine/System/Mutex.hpp>
#include <Burngine/System/Lock.hpp>
#include <sstream>
#include <Burngine/System/Error.hpp>

// For onOpenGlInit calls
#include <Burngine/Graphics/Texture/Texture2D.hpp>

namespace {

	burn::Mutex initMutex;    // Prevents instantiations before OpenGL is fully initialized
	burn::Mutex dataMutex;    // Prevents racing count modifications

	// Global count of GlEntities:
	unsigned int count = 0;

}

namespace burn {

	GlEntity::GlEntity() {

		Lock dataLock(dataMutex);
		if(++count == 1){
			dataMutex.unlock();

			Lock initLock(initMutex);
			// Init OpenGL and load all internal shaders
			priv::GlContext::globalInit();

			if(checkError())
				burnErr("An OpenGL error occured! Execution terminated.");

			BurnShaders::loadInternalShaders();

			// Call Burngine's onOpenGlInit methods
			Texture2D::onOpenGlInit();
		}

	}

	GlEntity::GlEntity(const GlEntity&) {
		++count;
	}

	GlEntity::~GlEntity() {

		Lock dataLock(dataMutex);
		if(--count == BurnShaders::COUNT){
			dataMutex.unlock();

			Lock initLock(initMutex);
			BurnShaders::releaseInternalShaders();
			priv::GlContext::globalCleanup();
		}

	}

	void GlEntity::ensureContext() {
		priv::GlContext::ensureContext();
	}

	bool GlEntity::checkError() {

		ensureContext();
		static GLenum error = glGetError();
		static bool triggered = false;
		while(error != GL_NO_ERROR){
			triggered = true;

			std::string what;
			switch (error) {
				case GL_INVALID_ENUM:
					what = "GL_INVALID_ENUM";
					break;
				case GL_INVALID_OPERATION:
					what = "GL_INVALID_OPERATION";
					break;
				case GL_INVALID_FRAMEBUFFER_OPERATION:
					what = "GL_INVALID_FRAMEBUFFER_OPERATION";
					break;
				case GL_OUT_OF_MEMORY:
					what = "GL_OUT_OF_MEMORY";
					break;
				case GL_STACK_OVERFLOW:
					what = "GL_STACK_OVERFLOW";
					break;
				case GL_STACK_UNDERFLOW:
					what = "GL_STACK_UNDERFLOW";
					break;
				default:
					what = "UNKNOWN_ERROR";
					break;
			}

			burnWarn("OpenGL error occured: " + what);
			error = glGetError();

		}

		return triggered;
	}

}
/* namespace burn */
