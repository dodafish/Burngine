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

#include <Burngine/System/Thread.hpp>
#include <Burngine/System/Error.hpp>
#include <pthread/pthread.h>

namespace {

#if defined(BURNGINE_OS_WINDOWS)
#include <windows.h>

	void platformSleep(const burn::Int64& millis) {
		Sleep(static_cast<DWORD>(millis)); // Expects milliseconds
	}

#else
#include <unistd.h>

	void platformSleep(const burn::Int64& millis){
		usleep(millis * 1000); // Expects microseconds
	}

#endif

}
/* namespace */

namespace burn {

void Thread::sleep(const Int64& millis) {
	platformSleep(millis);
}

Thread::Thread(	void* (*function)(void*),
				void* arg) :
m_thread(new pthread_t),
m_function(function),
m_arg(arg) {

}

Thread::~Thread() {
	delete (pthread_t*)m_thread;
}

void Thread::launch() {
	pthread_t* pthread = (pthread_t*)m_thread;
	if(pthread_create(pthread, NULL, m_function, &m_arg))
		burnErr("Failed to launch thread!");
}

void Thread::join() {
	pthread_t* pthread = (pthread_t*)m_thread;
	pthread_join(*pthread, NULL);
}

} /* namespace burn */
