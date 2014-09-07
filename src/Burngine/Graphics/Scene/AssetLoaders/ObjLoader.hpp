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

#ifndef OBJLOADER_HPP_
#define OBJLOADER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Graphics/Scene/Model.hpp>
#include <Burngine/Graphics/Scene/Mesh.hpp>
#include <Burngine/Graphics/Texture/Texture.hpp>
#include <Burngine/System/Math.hpp>
#include <string>
#include <vector>
#include <fstream>

namespace burn {
	namespace priv {

		/**
		 * @brief Loads OBJ assets from file
		 */
		class BURNGINE_API_EXPORT ObjLoader {
		public:

			/**
			 * @brief Load an OBJ from file and store the resulting
			 * Model into target.
			 * This assumes that the file has been checked already
			 */
			bool load(	const std::string& fileName,
						Model& target);

			/**
			 * @brief Reset loading cache
			 */
			void reset();

		private:

			/**
			 * @brief Load everything into cache
			 */
			bool loadIntoCache(const std::string& fileName);

			/**
			 * @brief Read the next line and format it.
			 * Read from f, stored into s
			 */
			bool nextLine(	std::fstream& f,
							std::string& s);

			bool parseObjectLine(const std::string& line);

			/**
			 * @brief Load mtllib into cache
			 */
			bool loadMtllib(const std::string& fileName);

			/**
			 * @brief Process the loaded cache and create the Model.
			 * Stored in m_model.
			 */
			bool processCache();

		private:

			Model m_model;

			std::string m_objectFileName;
			std::fstream m_objectFile;    // the *.obj file

			/**
			 * @brief Unprocessed mesh data
			 */
			struct MeshData {
				MeshData();
				std::vector<Vector3f> positions;
				std::vector<Vector3f> normals;
				std::vector<Vector2f> uvs;
				std::vector<int> indices;
				int componentCount;
				std::string materialName;
			};
			std::vector<MeshData> m_meshData;

		};

	} /* namespace priv */
} /* namespace burn */

#endif /* OBJLOADER_HPP_ */
