#pragma once
#include "Shapes/Polygon.h"

class ContextManager;

class Scene
{

public:
	Scene(ContextManager& contextManager);

	// Called once when this Scene or SceneContext is destroyed
	void OnDestroy();

	// Called once every frame
	void OnUpdate();

	void RunSimulation();

	void RenderPolygonPointsOnScene();
	void RenderConvexHullLinesOnScene();

	void RenderLineSweeper();
	void RenderLineSweepLineSegments();
	void RenderLineSweepVertices(std::vector<Shape::Event>& events);

	Shape::Polygon& GetShapePolygon();
	Shape::Polygon& GetConvexHull();

	bool& GetRenderOnlyConvexHullPoints();
	bool& GetRenderConvexHull();

	int& GetSimulationWait();
	int GetMaxSimulationWait();

	void SetConvexHullSimulationCompleted(bool completion);
	void SetLineSweepSimulationCompleted(bool completion);

	bool GetLineSweepSimulationCompleted() const;

	std::vector<Shape::Line> m_LineSegments;
	std::vector<glm::vec2> m_LineIntersections;
	std::vector<Shape::Event> m_LineSweep_Q;
	std::vector<Shape::Event> m_LineSweep_T;


private:
	// Scene's file name
	std::string m_SceneFileName{};

	Shape::Polygon m_Polygon;
	Shape::Polygon m_Hull;

	Shape::Line m_LineSweeper{ { -10000, 10000 }, { 10000, 10000 } };

	bool m_RenderOnlyConvexHullPoints = false;
	bool m_RenderConvexHull = true;
	bool m_ConvexHullCompleted = false;
	bool m_ConvexHullSimulationCompleted = false;
	int m_SimulationWait = 500;
	const int MAX_CONVEX_HULL_SIMULATION_WAIT = 10000;

	bool m_LineSweepSimulationCompleted = false;

	// Context References
	ContextManager& m_ContextManager;

	// true by default in order to run the Start() function in scripts for the first update loop
	// bool m_IsFirstRun = true;
};

// Scene Shared Ptr
using ScenePtr = std::shared_ptr<Scene>;