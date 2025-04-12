#pragma once

#include "Window/WindowContext.h"
#include "Graphics/GraphicsContext.h"
#include "Scene/SceneContext.h"
#include "Input/InputContext.h"
#include "Editor/EditorContext.h"

class ContextManager
{
public:
	ContextManager();

	WindowContext& GetWindowContext() { return m_WindowContext; }
	GraphicsContext& GetGraphicsContext() { return m_GraphicsContext; }
	EditorContext& GetEditorContext() { return m_EditorContext; }
	InputContext& GetInputContext() { return m_InputContext; }
	SceneContext& GetSceneContext() { return m_SceneContext; }

private:
	WindowContext m_WindowContext;
	GraphicsContext m_GraphicsContext;
	EditorContext m_EditorContext;
	InputContext m_InputContext;
	SceneContext m_SceneContext; // Initialize after all Contexts
};