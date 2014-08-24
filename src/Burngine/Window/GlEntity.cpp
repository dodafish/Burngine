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

#include <iostream>
#include <fstream>

namespace {

// Global count of GlEntities:
	unsigned int count = 0;

}

namespace burn {

	GlEntity::GlEntity() {

		std::cout << "GlEntities: " << count << "+\n";
		std::ofstream log("log.txt", std::ios::app);
		log << "GlEntities: " << count << "+\n";
		log.close();

		if(count == 0)
			priv::GlContext::globalInit();

		++count;

	}

	GlEntity::GlEntity(const GlEntity&) {
		std::cout << "GlEntities: " << count << "+\n";
		std::ofstream log("log.txt", std::ios::app);
		log << "GlEntities: " << count << "+\n";
		log.close();
		++count;
	}

	GlEntity::~GlEntity() {
		std::cout << "GlEntities: " << count << "-\n";
		std::ofstream log("log.txt", std::ios::app);
		log << "GlEntities: " << count << "-\n";
		log.close();
		--count;

		if(count == 0){
			priv::GlContext::globalCleanup();
		}

	}

	void GlEntity::ensureContext() {
		priv::GlContext::ensureContext();
	}

} /* namespace burn */
