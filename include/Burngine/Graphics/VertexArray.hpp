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

#ifndef VERTEXARRAY_HPP_
#define VERTEXARRAY_HPP_

#include <Burngine/Export.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/Window/GlEntity.hpp>
#include <map>

namespace burn {

	/**
	 * @brief API for OpenGL's vertex array object
	 */
	class BURNGINE_API_EXPORT VertexArray : public GlEntity {
	public:

		/**
		 * @brief Initializes
		 */
		VertexArray();

		/**
		 * @brief Copies and raises counter
		 */
		VertexArray(const VertexArray& other);

		/**
		 * @brief Cleans up if necessary and copies
		 */
		VertexArray& operator=(const VertexArray& other);

		/**
		 * @brief Cleans up generated IDs
		 */
		~VertexArray();

		/**
		 * @brief Bind vertex array.
		 * Don't forget to unbind!
		 */
		void bind() const;

		/**
		 * @brief Unbind vertex array
		 */
		void unbind() const;

		/**
		 * @brief Is the vertex array updated for this thread?
		 */
		bool needsUpdate() const;

		/**
		 * @brief Tell the vertex array that it is updated for this thread
		 */
		void setUpdated() const;

		/**
		 * @brief Set the update needed status to true. Thus, every thread
		 * should update the vertex array, again.
		 */
		void forceUpdateStatus() const;

	private:

		struct ThreadId;

		/**
		 * @brief Get the ThreadId for the calling thread
		 */
		ThreadId* getThreadId() const;

	private:

		/**
		 * @brief Record for per thread id
		 */
		struct ThreadId {
			ThreadId();
			GLuint id;
			bool isUpdated;
		};

		mutable std::map<void*, ThreadId> m_threadIds;    ///< Per thread IDs
		Uint32* m_count;
	};

} /* namespace burn */

#endif /* VERTEXARRAY_HPP_ */
