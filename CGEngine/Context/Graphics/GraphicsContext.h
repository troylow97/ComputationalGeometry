#pragma once

#include "Messages/messages.hpp"

class WindowContext;

class GraphicsContext
{
public:
	~GraphicsContext();

	bool CreateContext(glm::ivec2 windowSize);

	void ClearBuffer();

	void EnablePointsRender();

	void DisablePointsRender();

	void EnableLinesRender();

	void DisableLinesRender();

private:
};