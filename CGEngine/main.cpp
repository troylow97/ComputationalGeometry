#include "pch.h"

#ifndef _DEBUG
// Note that this needs to be included before glfw3.h
#include <Windows.h>
#endif
#include "CGEngine/CGEngine.h"

int main()
{
	CGEngine engine;

	engine.Run();

	return 0;
}