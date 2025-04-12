#include "pch.h"
#include <thread>
#include "CGEngine.h"

void CGEngine::Run()
{
	const WindowContext& windowContext = m_ContextManager.GetWindowContext();
	SceneContext& sceneContext = m_ContextManager.GetSceneContext();
	GraphicsContext& graphicsContext = m_ContextManager.GetGraphicsContext();
	EditorContext& editorContext = m_ContextManager.GetEditorContext();
	InputContext& inputContext = m_ContextManager.GetInputContext();

	simulationThread = std::thread(&CGEngine::RunSimulation, this);

	while (!windowContext.ShouldWindowClose()) {
		graphicsContext.ClearBuffer();

		windowContext.updateViewportSize();

		const auto scenePtr = sceneContext.GetActiveScene();

		inputContext.Update();

		if (inputContext.WasMouseClicked(GLFW_MOUSE_BUTTON_LEFT)) {
			glm::vec2 mousePos = inputContext.GetMousePosition();
			std::cout << "Mouse clicked at: " << mousePos.x << ", " << mousePos.y << std::endl;
		}

		// Start of Editor Frame
		editorContext.BeginEditorFrame();
		editorContext.RenderWidgets();

		// End of Editor Frame
		editorContext.EndEditorFrame();

		// Render Edges ======================
		// Enable renderer to lines mode
		graphicsContext.EnableLinesRender();

		// Render the convex hull when required
		if (scenePtr->GetRenderConvexHull())
			scenePtr->RenderConvexHullLinesOnScene();

		// Render lines for Line Sweep Algorithm when required
		if (!scenePtr->GetLineSweepSimulationCompleted())
			scenePtr->RenderLineSweeper();

		scenePtr->RenderLineSweepLineSegments();

		// After rendering lines, disable lines mode
		graphicsContext.DisableLinesRender();
		//=====================================


		// Render Vertices ====================
		// Enable points mode for renderer
		graphicsContext.EnablePointsRender();

		// Render points for Convex Hull Algorithm when required
		scenePtr->RenderPolygonPointsOnScene();

		// Set to render white for Q vertices
		glColor3f(1, 1, 1);
		// Render Q Vertices for Line Sweep
		scenePtr->RenderLineSweepVertices(scenePtr->m_LineSweep_Q);

		// Set to render yellow for T vertices
		glColor3f(1, 1, 0);
		// Render T Vertices for Line Sweep
		scenePtr->RenderLineSweepVertices(scenePtr->m_LineSweep_T);

		// After rendering lines, disable points mode
		graphicsContext.DisablePointsRender();
		//=====================================

		windowContext.SwapBuffers();

		// Poll pending window events
		windowContext.PollEvents();
	}
}

void CGEngine::RunSimulation() {
	const WindowContext& windowContext = m_ContextManager.GetWindowContext();
	SceneContext& sceneContext = m_ContextManager.GetSceneContext();

	while (!windowContext.ShouldWindowClose())
		sceneContext.GetActiveScene()->RunSimulation();
}

CGEngine::~CGEngine() {
	simulationThread.detach();
}
