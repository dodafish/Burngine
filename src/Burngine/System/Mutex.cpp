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

#include <Burngine/System/Mutex.hpp>
#include <pthread/pthread.h>

namespace {

	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_t unlockedCondition = PTHREAD_COND_INITIALIZER;

} /* namespace */

namespace burn {

	Mutex::Mutex() :
	m_isLocked(false),
	m_owner(NULL) {

	}

	void Mutex::lock() {

		void* me = pthread_self().p;

		if(m_owner == me)
			return;

		m_queue.push(me);

		for(;;){
			while(m_isLocked)
				pthread_cond_wait(&unlockedCondition, &mutex);
			if(m_queue.front() == me){
				m_isLocked = true;
				m_owner = me;
				m_queue.pop();
				break;
			}
		}

	}

	void Mutex::unlock() {

		m_owner = NULL;
		m_isLocked = false;

		pthread_mutex_unlock(&mutex);
		pthread_cond_signal(&unlockedCondition);

	}

} /* namespace burn */
