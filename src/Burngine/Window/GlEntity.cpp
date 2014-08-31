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
#include <Burngine/Graphics/Texture/TextureLoader.hpp>
#include <Burngine/System/Mutex.hpp>
#include <Burngine/System/Lock.hpp>

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
			BurnShaders::loadInternalShaders();
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
			priv::TextureLoader::cleanup();
			BurnShaders::releaseInternalShaders();
			priv::GlContext::globalCleanup();
		}

	}

	void GlEntity::ensureContext() {
		priv::GlContext::ensureContext();
	}

}
/* namespace burn */
