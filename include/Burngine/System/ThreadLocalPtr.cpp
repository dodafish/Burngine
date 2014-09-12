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

#include <Burngine/System/ThreadLocalPtr.hpp>
#include <Burngine/System/Thread.hpp>

namespace burn {

	template<class T>
	void ThreadLocalPtr<T>::set(T* pointer) {

		void* me = Thread::current();

		for(size_t i = 0; i < m_pointers.size(); ++i){
			if(m_pointers[i].first == me){
				m_pointers[i].second = pointer;
				return;
			}
		}

		m_pointers.push_back(std::pair<void*, T*>(me, pointer));
	}

	template<class T>
	T* ThreadLocalPtr<T>::get() {

		void* me = Thread::current();

		for(size_t i = 0; i < m_pointers.size(); ++i){
			if(m_pointers[i].first == me){
				return m_pointers[i].second;
			}
		}

		return NULL;
	}

	template<class T>
	void ThreadLocalPtr<T>::clear() {
		m_pointers.clear();
	}

} /* namespace burn */
