#include "pch.h"
#include <iostream>
#include "GraphicsContext.h"
#include "Context/ContextManager.h"
#include "Context/Window/WindowContext.h"

void APIENTRY ErrorCallback(
	[[maybe_unused]] GLenum source, GLenum type,
	[[maybe_unused]] GLuint id, GLenum severity,
	[[maybe_unused]] GLsizei length, const GLchar* message,
	[[maybe_unused]] const void* userParam)
{
	if (type == GL_DEBUG_TYPE_ERROR) {
		printf("[GL Error]\nType: 0x%x\nSeverity: 0x%x\nMessage: %s\n\n", type, severity, message);
#ifdef _DEBUG
		__debugbreak(); // Break on GL error
#endif
	}
}

GraphicsContext::~GraphicsContext() {
}

bool GraphicsContext::CreateContext(glm::ivec2 windowSize)
{
	// Init OpenGL Context
	if (const GLenum err = glewInit(); err != GLEW_OK) {
		std::cout << glewGetErrorString(err) << std::endl;
		//printf("Unable to initialize GLEW: %s\n", glewGetErrorString(err));
		return false;
	}

	// Enable OpenGL error callback
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS); // Allows call-stack cycling during an error for debugging
	glDebugMessageCallback(ErrorCallback, nullptr);

	// Print GPU Information
	printf("Using GLEW Version: %s\n", glewGetString(GLEW_VERSION));
	printf("GPU Vendor: %s\n", glGetString(GL_VENDOR));
	printf("GL Renderer: %s\n", glGetString(GL_RENDERER));
	printf("GL Version: %s\n", glGetString(GL_VERSION));

	// specifies the part of the window to which OpenGL will draw (in pixels), convert from normalised to pixels
	glViewport(0, 0, windowSize.x, windowSize.y);

	// projection matrix defines the properties of the camera that views the objects in the world coordinate frame.
	// Here you typically set the zoom factor, aspect ratio and the near and far clipping planes
	glMatrixMode(GL_PROJECTION);

	// replace the current matrix with the identity matrix and starts us a fresh because matrix transforms such as glOrpho and glRotate cumulate, basically puts us at (0, 0, 0)
	glLoadIdentity();

	// essentially set coordinate system
	glOrtho(static_cast<GLdouble>(-windowSize.x) / 2, static_cast<GLdouble>(windowSize.x) / 2, static_cast<GLdouble>(-windowSize.y) / 2, static_cast<GLdouble>(windowSize.y) / 2, 0, 1);

	// (default matrix mode) modelview matrix defines how your objects are transformed (meaning translation, rotation and scaling) in your world
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	std::cout << Messages::GRAPHICS_CONTEXT_CREATION_SUCCESS << std::endl;

	return true;
}

void GraphicsContext::ClearBuffer()
{
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void GraphicsContext::EnablePointsRender()
{
	// make the point circular
	glEnable(GL_POINT_SMOOTH);

	// tell OpenGL that you're using a vertex array for fixed-function attribute
	glEnableClientState(GL_VERTEX_ARRAY);
}

void GraphicsContext::DisablePointsRender()
{
	// tell OpenGL that you're finished using the vertex array attribute
	glDisableClientState(GL_VERTEX_ARRAY);

	// stop the smoothing to make the points circular
	glDisable(GL_POINT_SMOOTH);
}

void GraphicsContext::EnableLinesRender()
{
	glEnable(GL_LINE_SMOOTH);
	glPushAttrib(GL_LINE_BIT);
	glLineWidth(6);
	glLineStipple(1, 0x00FF);
	glEnableClientState(GL_VERTEX_ARRAY);
}

void GraphicsContext::DisableLinesRender()
{
	glDisableClientState(GL_VERTEX_ARRAY);
	glPopAttrib();
	glDisable(GL_LINE_SMOOTH);
}
