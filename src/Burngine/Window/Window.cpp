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
#include <Burngine/Window/GlContext.hpp>
#include <Burngine/OpenGL.hpp>
#include <Burngine/System/Thread.hpp>
#include <iostream>

namespace burn {

	Window::Window() :
	m_impl(NULL),
	m_title("Burngine App"),
	m_style(NORMAL),
	m_context(NULL),
	m_framerateLimit(0) {
	}

	Window::~Window() {

		std::cout << "Destroying window...\n";

		// Close the window if needed
		close();

	}

	bool Window::create(const VideoMode& videoMode,
						const std::string& title,
						const Style& style,
						bool fullscreen) {

		// Close a possibly created window before creating
		// a new one
		close();

		// Store attributes
		m_videoMode = videoMode;
		m_title = title;
		m_style = style;

		// Create a new window
		m_impl = priv::WindowImpl::create(m_videoMode, m_title, m_style, fullscreen);

		// Create a bound context
		m_context = priv::GlContext::create(this);

		return true;
	}

	bool Window::create() {

		// Close a possibly created window before creating
		// a new one
		close();

		// Create a new window
		m_impl = priv::WindowImpl::create(m_videoMode, m_title, m_style, false);

		m_context = priv::GlContext::create(this);

		return true;
	}

	void Window::close() {

		// Delete the context and the window
		delete m_context;
		delete m_impl;

		m_context = NULL;
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

	void Window::clear(const Vector4f& color) {
		if(m_context){
			m_context->setActive();
			glClearColor(color.r, color.g, color.b, color.a);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		}
	}

	void Window::display() {
		if(m_context){

			if(m_framerateLimit != 0){

				double remaining = (1.0 / m_framerateLimit) - m_clock.getElapsedTime().asSeconds();

				if(remaining > 0.0)
					Thread::sleep(remaining * 1000);

				m_clock.reset();
			}

			m_context->swapBuffers();
		}

	}

	void Window::setVideoMode(const VideoMode& videoMode) {

		m_videoMode = videoMode;

		if(m_impl){
			m_impl->setDimensions(Vector2i(m_videoMode.getDimensions()));
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

	const Window::Style& Window::getStyle() const {
		return m_style;
	}

	void Window::setFramerateLimit(const Uint32& framerate) {
		m_framerateLimit = framerate;
	}

	const Uint32& Window::getFramerateLimit() const {
		return m_framerateLimit;
	}

	WindowHandle Window::getWindowHandle() const {
		if(m_impl){
			return m_impl->getWindowHandle();
		}
		return NULL;
	}

	bool Window::prepare() const {
		if(m_context){
			m_context->setActive(true);
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
			glViewport(0, 0, m_videoMode.getWidth(), m_videoMode.getHeight());
			return true;
		}
		return false;
	}

	Matrix4f Window::getOrtho() const {
		return glm::ortho(	0.f,
							static_cast<float>(m_videoMode.getWidth()),
							static_cast<float>(m_videoMode.getHeight()),
							0.f);
	}

	const Vector2ui& Window::getDimensions() const{
		return m_videoMode.getDimensions();
	}

} /* namespace burn */
