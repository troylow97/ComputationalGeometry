#include "pch.h"
#include <Windows.h>
#include <iostream>
#include "Context/ContextManager.h"

// Error callback information
void GLErrorCallback(int errorCode, char const* description) {
	std::cout << "GLFW [Error " << errorCode << "] - " << description << std::endl;
}

bool WindowContext::CreateContext(ContextManager& ctxMgr, const WindowSettings&) {
	if (!glfwInit())
	{
		std::cout << Messages::GLFW_INIT_FAILURE << std::endl;
		return false;
	}

	glfwSetErrorCallback(GLErrorCallback);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	glfwWindowHint(GLFW_RED_BITS, 8);
	glfwWindowHint(GLFW_GREEN_BITS, 8);
	glfwWindowHint(GLFW_BLUE_BITS, 8);
	glfwWindowHint(GLFW_ALPHA_BITS, 8);

	m_WindowPtr = glfwCreateWindow
	(
		WindowSettings::defaultWindowSize.x,
		WindowSettings::defaultWindowSize.y,
		m_WindowTitle.c_str(),
		nullptr, nullptr
	);

	if (!m_WindowPtr)
	{
		std::cout << Messages::GLFW_CREATE_WINDOW_FAILURE << std::endl;
		glfwTerminate();
		return false;
	}

	// Enable Window Context
	glfwMakeContextCurrent(m_WindowPtr);

	// Allow GLFW callbacks to access ContextManager
	glfwSetWindowUserPointer(m_WindowPtr, &ctxMgr);

	glfwSetWindowTitle(m_WindowPtr, WindowSettings::windowTitle.c_str());
	m_WindowTitle = WindowSettings::windowTitle;

	std::cout << Messages::WINDOW_CONTEXT_CREATION_SUCCESS << std::endl;

	return true;
}

bool WindowContext::ShouldWindowClose() const {
	return glfwWindowShouldClose(m_WindowPtr);
}

void WindowContext::updateViewportSize() const {
	int width, height;
	glfwGetWindowSize(m_WindowPtr, &width, &height);

	// If window is minimized, we do not update the viewport. Else, it will crash at glMatrixMode()
	if (width == 0 && height == 0)
		return;

	glViewport(0, 0, width, height);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(static_cast<GLdouble>(-width) / 2, static_cast<GLdouble>(width) / 2, static_cast<GLdouble>(-height) / 2, static_cast<GLdouble>(height) / 2, 0, 1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void WindowContext::SwapBuffers() const {
	glfwSwapBuffers(m_WindowPtr);
}

void WindowContext::PollEvents() const {
	glfwPollEvents();
}

GLFWwindow* WindowContext::GetWindow() const {
	return m_WindowPtr;
}

// Cleans up and destroys window
WindowContext::~WindowContext() {
	if (m_WindowPtr)
	{
		glfwDestroyWindow(m_WindowPtr);
		glfwTerminate();
	}
}