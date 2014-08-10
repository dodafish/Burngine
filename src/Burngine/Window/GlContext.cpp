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

#include <vector>

#if defined(BURNGINE_OS_WINDOWS)

#include <Burngine/Window/Win32/WglContext.hpp>
typedef burn::priv::WglContext GlContextType;

#endif

/////////////////////////////////////////////////////////////////////////////

namespace {

GlContextType* sharedContext = NULL;    // Usually always inactive

burn::priv::GlContext* currentContext = NULL;
burn::priv::GlContext* internalContext = NULL;

std::vector<burn::priv::GlContext*> internalContexts;    // For cleaning up

bool hasInternalContext() {
	if(!internalContext)
		return false;
	return true;
}

burn::priv::GlContext* getInternalContext() {

	if(!hasInternalContext()){
		internalContext = burn::priv::GlContext::create();
		internalContexts.push_back(internalContext);
	}

	return internalContext;
}

}

namespace burn {
namespace priv {

void GlContext::globalInit() {

	sharedContext = new GlContextType(NULL);
	sharedContext->setActive(false);

}

void GlContext::globalCleanup() {

	currentContext = NULL;

	delete sharedContext;
	sharedContext = NULL;

	for(size_t i = 0; i < internalContexts.size(); ++i)
		delete internalContexts[i];
	internalContexts.clear();

}

void GlContext::ensureContext() {

	// A context is active
	if(currentContext)
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
	if(sharedContext)
		setActive(false);
}

void GlContext::setActive(bool active) {

	if(active){

		if(this == currentContext){
			// Context is already active
			return;
		}

		makeCurrent();
		currentContext = this;

	}else{

		if(this != currentContext){
			// Already inactive, do nothing
			return;
		}

		// Ensure an active context anyways. For following OpenGL calls
		getInternalContext()->setActive();

	}

}

} /* namespace priv */
} /* namespace burn */
