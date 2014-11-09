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

#ifndef VERTEXBUFFER_HPP_
#define VERTEXBUFFER_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <Burngine/OpenGL.hpp>
#include <vector>

namespace burn {

	/**
	 * @brief API for OpenGL's vertex buffer objects
	 */
	class BURNGINE_API_EXPORT VertexBuffer : public GlEntity {
	public:

		/**
		 * @brief Initialize attributes
		 */
		VertexBuffer();

		/**
		 * @brief Create an instance and copy data.
		 */
		VertexBuffer(const VertexBuffer& other);

		/**
		 * @brief Release resources
		 */
		~VertexBuffer();

		/**
		 * Copy data
		 */
		VertexBuffer& operator=(const VertexBuffer& other);

		/**
		 * @brief Reset added data
		 */
		void reset();

		/**
		 * @brief Add data
		 *
		 * @param data Data
		 * @param size Data's size
		 */
		void addData(	const void* data,
						const unsigned int& size);

		/**
		 * @brief Get the saved data
		 *
		 * @return Vertex data
		 */
		const std::vector<GLbyte>& getData() const;

		/**
		 * @brief Bind the VBO (and upload data). Don't forget to unbind!
		 */
		void bind() const;

		/**
		 * @brief Unbind the VBO
		 */
		void unbind() const;

	private:
		GLuint m_id;    ///< VBO's id
		std::vector<GLbyte> m_data;    ///< VBO's data
		mutable bool m_isDataUploaded;    ///< VBO uploaded to graphic card?
	};

} /* namespace burn */

#endif /* VERTEXBUFFER_HPP_ */
