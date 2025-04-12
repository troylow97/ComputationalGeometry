#include "pch.h"
#include "ContextManager.h"

ContextManager::ContextManager() :
	m_SceneContext{ *this },
	m_EditorContext{ *this }
{
	WindowSettings windowSettings;
	GraphicsSettings graphicsSettings;

	if (!m_WindowContext.CreateContext(*this, windowSettings)) {
		std::cout << Messages::WINDOW_CONTEXT_CREATION_FAILED << std::endl;
		return;
	}

	if (!m_GraphicsContext.CreateContext(WindowSettings::defaultWindowSize)) {
		std::cout << Messages::GRAPHICS_CONTEXT_CREATION_FAILED << std::endl;
		return;
	}

	if (!m_InputContext.CreateContext(m_WindowContext.GetWindow())) {
		std::cout << Messages::INPUT_CONTEXT_CREATION_FAILED << std::endl;
		return;
	}

	if (!m_EditorContext.CreateContext()) {
		std::cout << Messages::EDITOR_CONTEXT_CREATION_FAILED << std::endl;
		return;
	}
}