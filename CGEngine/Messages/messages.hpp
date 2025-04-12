#ifndef MESSAGES_H
#define MESSAGES_H

#include <string>

namespace Messages {
	const std::string FAILED_TO_CREATE_WINDOW = "Failed to create GLFW window.";
	const std::string GLFW_INIT_FAILURE = "glfwInit() - GLFW init failure";
	const std::string GLFW_CREATE_WINDOW_FAILURE = "glfwCreateWindow() - GLFW unable to create window context.";
	const std::string WINDOW_CONTEXT_CREATION_SUCCESS = "Window Context successfully created.";
	const std::string WINDOW_CONTEXT_CREATION_FAILED = "Window Context creation failed.";
	const std::string GRAPHICS_CONTEXT_CREATION_SUCCESS = "Graphics Context successfully created.";
	const std::string GRAPHICS_CONTEXT_CREATION_FAILED = "Graphics Context creation failed.";
	const std::string INPUT_CONTEXT_CREATION_FAILED = "Input Context creation failed.";
	const std::string EDITOR_CONTEXT_CREATION_SUCCESS = "Editor Context successfully created.";
	const std::string EDITOR_CONTEXT_CREATION_FAILED = "Editor Context creation failed.";
}

#endif