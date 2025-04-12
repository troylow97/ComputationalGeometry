#pragma once

#include "Context/ContextManager.h"

class CGEngine {
public:
	CGEngine() = default;

	~CGEngine();

	ContextManager& GetContextManager() { return m_ContextManager; }

	void Init();

	void Run();

	void RunSimulation();

private:
	ContextManager m_ContextManager{};
	std::thread simulationThread;
};