#pragma once

#include <GLFW/glfw3.h>
#include "Messages/messages.hpp"
#include "Context/Settings/ContextSettings.h"

class ContextManager;

class WindowContext
{
public:
	bool CreateContext(ContextManager& ctxMgr, const WindowSettings& windowSettings);

	bool ShouldWindowClose() const;

	void UpdateViewportSize() const;

	glm::vec2 GetWindowSize() const;

	// Swaps the front and back buffers of the Window
	void SwapBuffers() const;

	// // Tells GLFW to process events, particularly pending window events.
	void PollEvents() const;

	GLFWwindow* GetWindow() const;

	~WindowContext();

private:
	std::string m_WindowTitle{};
	GLFWwindow* m_WindowPtr{};
};