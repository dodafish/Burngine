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

#ifndef MODELLOADER_HPP_
#define MODELLOADER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Model.hpp>
#include <string>
#include <map>

namespace burn {
	namespace priv {

		class BURNGINE_API_EXPORT ModelLoader {
		public:

			/**
			 * @brief Load a model from file
			 *
			 * @return true on success
			 */
			static bool loadFromFile(	const std::string& fileName,
										Model& target);

		private:

			enum FileType {
				UNKNOWN,    ///< Unsupported type
				OBJ    ///< Wavefront (*.obj)
			};

			static FileType checkFileType(const std::string& fileName);

		private:
			static std::map<size_t, Model> m_models;    ///< Loaded models
		};

	} /* namespace priv */
} /* namespace burn */

#endif /* MODELLOADER_HPP_ */
