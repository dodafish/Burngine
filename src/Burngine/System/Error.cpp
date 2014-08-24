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

#include <Burngine/System/Error.hpp>
#include <iostream>
#include <sstream>

void msgBox(const std::string& msg);
std::string finalString(const std::string& msg);

#if defined(BURNGINE_OS_WINDOWS)

#include <windows.h>
#include <tchar.h>

std::string getWin32ErrorMsg() {

	DWORD dw = GetLastError();
	if(dw == 0){
		return "No win32 error detected (Code: 0)";
	}
	LPVOID lpMsgBuf;
	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
	NULL,
					dw, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&lpMsgBuf, 0, NULL);

	std::string msg = (LPSTR)lpMsgBuf;
	LocalFree(lpMsgBuf);

	std::stringstream msgCodeCombine;
	msgCodeCombine << msg << " (Code: " << dw << ")";

	return msgCodeCombine.str();
}

void msgBox(const std::string& msg) {
	MessageBox(NULL, msg.c_str(), "Burngine Error", MB_ICONERROR | MB_OK);
}

std::string finalString(const std::string& msg) {
	return "\"" + msg + "\"\nWin32-Error: " + getWin32ErrorMsg();
}

#else

void msgBox(const std::string& msg){}

std::string finalString(const std::string& msg){
	return msg;
}

#endif

namespace burn {

	void Error::log(const std::string& msg,
					const char* fileFullPath,
					int line) {

		std::string file = fileFullPath;
		size_t start = file.find("Burngine");
		file.erase(0, start + 8);
		file = "..." + file;

		std::stringstream ss;
		ss << line;
		std::string s = "Error: " + finalString(msg) + "\n\n(File: " + file + "@" + ss.str() + ")";

		// Also print the error into console
		std::cerr << s << "\n";

		msgBox("Burngine has run into an error.\n\n" + s);

		// Interrupt execution
		exit(8074);

	}

} /* namespace burn */
