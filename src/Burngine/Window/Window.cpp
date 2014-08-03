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

#include <Burngine/Window/Window.hpp>
#include <Burngine/Window/WindowImpl.hpp>

namespace burn {

Window::Window() :
m_impl(NULL),
m_title("Burngine App"),
m_style(NORMAL) {
}

Window::~Window() {

	// Close the window if needed
	close();

}

bool Window::create(const VideoMode& videoMode,
					const std::string& title,
					const Style& style) {

	// Close a possibly created window before creating
	// a new one
	close();

	// Store attributes
	m_videoMode = videoMode;
	m_title = title;
	m_style = style;

	// Create a new window
	m_impl = priv::WindowImpl::create(m_videoMode, m_title, m_style);

	return m_impl != NULL;
}

bool Window::create() {

	// Close a possibly created window before creating
	// a new one
	close();

	// Create a new window
	m_impl = priv::WindowImpl::create(m_videoMode, m_title, m_style);

	return m_impl->creationSucceeded();
}

void Window::close() {

	// Delete the window
	delete m_impl;
	m_impl = NULL;

}

bool Window::isOpen() const {
	return m_impl != NULL;
}

bool Window::pollEvent(Event& event) {

	if(m_impl && m_impl->popEvent(event)){
		return true;
	}

	return false;
}

void Window::setVideoMode(const VideoMode& videoMode) {

	m_videoMode = videoMode;

	if(m_impl){
		m_impl->setDimensions(m_videoMode.getDimensions());
	}

}

const VideoMode& Window::getVideoMode() const {
	return m_videoMode;
}

void Window::setTitle(const std::string& title) {

	m_title = title;

	if(m_impl){
		m_impl->setTitle(m_title);
	}

}

const std::string& Window::getTitle() const {
	return m_title;
}

void Window::setStyle(const Style& style) {
	m_style = style;
}

const Window::Style& Window::getStyle() const {
	return m_style;
}

} /* namespace burn */
