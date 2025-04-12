#include "pch.h"
#include "SceneContext.h"

SceneContext::SceneContext(ContextManager& contextManager) :
	m_ContextManager{ contextManager } {
	m_ActiveScene = std::make_shared<Scene>(m_ContextManager);
}

SceneContext::~SceneContext() {
	if (m_ActiveScene)
		m_ActiveScene->OnDestroy();
}

ScenePtr SceneContext::GetActiveScene() const {
	return m_ActiveScene;
}

void SceneContext::Update() {
	if (m_ActiveScene)
		m_ActiveScene->OnUpdate();
}

ContextManager& SceneContext::GetContextManager() const {
	return m_ContextManager;
}