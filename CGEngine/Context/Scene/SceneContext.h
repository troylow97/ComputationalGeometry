#pragma once
#include "Scene.h"
#include <memory>

class SceneContext
{
public:
	SceneContext(ContextManager& contextManager);
	~SceneContext();

	// Returns the current Scene, nullptr if there is no loaded Scene
	ScenePtr GetActiveScene() const;

	// Updates the current Scene
	void Update();

	ContextManager& GetContextManager() const;

private:
	ScenePtr m_ActiveScene;

	ContextManager& m_ContextManager;
};