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

#include <Burngine/Window/GlContext.hpp>
#include <Burngine/Window/Window.hpp>
#include <Burngine/System/ThreadLocalPtr.hpp>
#include <Burngine/System/Mutex.hpp>
#include <Burngine/System/Lock.hpp>

#include <vector>
#include <iostream>

#if defined(BURNGINE_OS_WINDOWS)

#include <Burngine/Window/Win32/WglContext.hpp>
typedef burn::priv::WglContext GlContextType;

#elif defined(BURNGINE_OS_LINUX)
#error GL Contexts are not supported on Linux

#elif defined(BURNGINE_OS_MAC)
#error GL Contexts are not supported on MacOS

#endif

/////////////////////////////////////////////////////////////////////////////

namespace {

	GlContextType* sharedContext = NULL;    // Usually always inactive
	burn::ThreadLocalPtr<burn::priv::GlContext> currentContext;
	burn::ThreadLocalPtr<burn::priv::GlContext> internalContext;
	std::vector<burn::priv::GlContext*> internalContexts;    // For cleaning up

	burn::Mutex mutex;

	burn::priv::GlContext* getInternalContext() {

		if(internalContext.get() == NULL){
			burn::Lock lock(mutex);

			internalContext.set(burn::priv::GlContext::create());
			internalContexts.push_back(internalContext.get());
		}
		return internalContext.get();
	}

}

namespace burn {
	namespace priv {

		void GlContext::globalInit() {

			std::cout << "Initializing OpenGL...\n";

			burn::Lock lock(mutex);

			sharedContext = new GlContextType(NULL);
			sharedContext->setActive(false);

			std::cout << "Initialized OpenGL.\n";

		}

		void GlContext::globalCleanup() {

			std::cout << "Cleaning up OpenGL...\n";

			burn::Lock lock(mutex);

			currentContext.set(NULL);

			delete sharedContext;
			sharedContext = NULL;

			for(size_t i = 0; i < internalContexts.size(); ++i)
				delete internalContexts[i];
			internalContexts.clear();

			// Clear the thread dependend associations
			internalContext.clear();
			currentContext.clear();

			std::cout << "Cleaned up OpenGL.\n";

		}

		void GlContext::ensureContext() {

			// A context is active
			if(currentContext.get())
				return;

			getInternalContext()->setActive();

		}

		GlContext* GlContext::create() {
			return new GlContextType(sharedContext->getRC());
		}

		GlContext* GlContext::create(const Window* window) {
			return window == NULL ? NULL :
									new GlContextType(sharedContext->getRC(), window);
		}

		GlContext::~GlContext() {
			// Deactivate this context and ensure another one, unless
			// we are in globalCleanup()
			if(sharedContext){

				// Make sure that we don't accidently activate ourselves
				// by activating the internal context
				if(this == internalContext.get())
					internalContext.set(NULL);

				setActive(false);
			}
		}

		void GlContext::setActive(bool active) {

			if(active){

				if(this == currentContext.get()){
					// Context is already active
					return;
				}

				makeCurrent();
				currentContext.set(this);

			}else{

				if(this != currentContext.get()){
					// Already inactive, do nothing
					return;
				}

				// Ensure an active context anyways. For following OpenGL calls
				getInternalContext()->setActive();

			}

		}

	} /* namespace priv */
} /* namespace burn */
