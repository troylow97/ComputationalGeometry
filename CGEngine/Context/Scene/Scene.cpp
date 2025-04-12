#include "pch.h"
#include "Scene.h"
#include "Context/ContextManager.h"
#include "Algorithms/Algorithms.h"
#include <thread>
#include <chrono>
#include <vector>

Scene::Scene(ContextManager& contextManager) :
	m_ContextManager{ contextManager },
	m_Polygon{},
	m_Hull{},
	m_LineSegments()
{}

void Scene::OnDestroy() {
}

void Scene::OnUpdate(){
}

void Scene::RunSimulation() {
	if (!m_ConvexHullSimulationCompleted) {
		m_Hull.clearVertices();
		m_ConvexHullCompleted = false;
		std::this_thread::sleep_for(std::chrono::milliseconds(m_SimulationWait));

		FormConvexHull(m_Polygon, m_Hull, m_SimulationWait);

		m_ConvexHullCompleted = true;
		m_ConvexHullSimulationCompleted = true;
	}

	if (!m_LineSweepSimulationCompleted) {
		// Before simulating, we clear the lines intersections and rendering vertices
		m_LineIntersections.clear();
		m_LineSweep_Q.clear();
		m_LineSweep_T.clear();
		std::this_thread::sleep_for(std::chrono::milliseconds(m_SimulationWait));

		//Add graphics and line sweep algo here
		LineSweepAlgorithm(m_LineSegments, m_LineSweeper, m_LineSweep_Q, m_LineSweep_T, m_LineIntersections, m_SimulationWait);
		m_LineSweeper.GetVerticesStart() = { -10000, 10000 };
		m_LineSweeper.GetVerticesEnd() = { 10000, 10000 };
		m_LineSweepSimulationCompleted = true;
	}
}

void Scene::RenderPolygonPointsOnScene() {
	Shape::Polygon shapeToRender = m_Polygon;

	if (m_RenderOnlyConvexHullPoints)
		shapeToRender = m_Hull;

	const std::vector<glm::vec2> verticesToRender = shapeToRender.getVertexList();
	const std::vector<glm::vec2> pointsInConvexHull = m_Hull.getVertexList();

	for (int i = 0; i < static_cast<int>(shapeToRender.getNumOfVertices()); ++i)
	{
		const GLfloat pointToRender[] = { static_cast<GLfloat>(verticesToRender[i].x), static_cast<GLfloat>(verticesToRender[i].y)};

		if (std::find(pointsInConvexHull.begin(), pointsInConvexHull.end(), verticesToRender[i]) != pointsInConvexHull.end() && m_RenderConvexHull) {
			glPointSize(30);
			glColor3f(1, 1, 1);
		}
		else {
			glPointSize(25);
			glColor3f(1, 0, 0);
		}

		// point to the vertices to be used
		glVertexPointer(2, GL_FLOAT, 0, pointToRender);

		// draw the vertices
		glDrawArrays(GL_POINTS, 0, 1);
	}
}

void Scene::RenderConvexHullLinesOnScene() {
	const std::vector<glm::vec2> verticesToRender = m_Hull.getVertexList();

	for (int i = 0; i < static_cast<int>(m_Hull.getNumOfVertices()); ++i)
	{
		GLfloat lineVertices[6];

		if (i != static_cast<int>(m_Hull.getNumOfVertices()) - 1 && m_ConvexHullCompleted)
		{
			lineVertices[0] = verticesToRender[i].x;
			lineVertices[1] = verticesToRender[i].y;
			lineVertices[2] = 0;
			lineVertices[3] = verticesToRender[i + 1].x;
			lineVertices[4] = verticesToRender[i + 1].y;
			lineVertices[5] = 0;
		}

		else
		{
			lineVertices[0] = verticesToRender[i].x;
			lineVertices[1] = verticesToRender[i].y;
			lineVertices[2] = 0;
			lineVertices[3] = verticesToRender[0].x;
			lineVertices[4] = verticesToRender[0].y;
			lineVertices[5] = 0;
		}

		glColor3f(1, 1, 1);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, lineVertices);
		glDrawArrays(GL_LINES, 0, 2);
	}
}

void Scene::RenderLineSweeper() {
	GLfloat lineVertices[6];

	lineVertices[0] = m_LineSweeper.GetTop().x;
	lineVertices[1] = m_LineSweeper.GetTop().y;
	lineVertices[2] = 0;
	lineVertices[3] = m_LineSweeper.GetBottom().x;
	lineVertices[4] = m_LineSweeper.GetBottom().y;
	lineVertices[5] = 0;

	glColor3f(1, 0, 0);
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, lineVertices);
	glDrawArrays(GL_LINES, 0, 2);
}

void Scene::RenderLineSweepLineSegments() {
	for (int i = 0; i < static_cast<int>(m_LineSegments.size()); ++i)
	{
		GLfloat lineVertices[6];

		lineVertices[0] = m_LineSegments[i].GetTop().x;
		lineVertices[1] = m_LineSegments[i].GetTop().y;
		lineVertices[2] = 0;
		lineVertices[3] = m_LineSegments[i].GetBottom().x;
		lineVertices[4] = m_LineSegments[i].GetBottom().y;
		lineVertices[5] = 0;

		glColor3f(1, 1, 1);
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, 0, lineVertices);
		glDrawArrays(GL_LINES, 0, 2);
	}
}

void Scene::RenderLineSweepVertices(std::vector<Shape::Event>& events) {
	// We make a copy of events to prevent an update while rendering is in progress.
	std::vector<Shape::Event> eventsToRender = events;

	glPointSize(25);

	// First, we reconstruct t
	for (const auto& event : events)
	{
		const GLfloat pointToRender[] = { static_cast<GLfloat>(event.point.x), static_cast<GLfloat>(event.point.y) };

		// point to the vertices to be used
		glVertexPointer(2, GL_FLOAT, 0, pointToRender);

		// draw the vertices
		glDrawArrays(GL_POINTS, 0, 1);
	}
}

Shape::Polygon& Scene::GetShapePolygon() {
	return m_Polygon;
}

Shape::Polygon& Scene::GetConvexHull() {
	return m_Hull;
}

bool& Scene::GetRenderOnlyConvexHullPoints() {
	return m_RenderOnlyConvexHullPoints;
}

bool& Scene::GetRenderConvexHull() {
	return m_RenderConvexHull;
}

int& Scene::GetSimulationWait() {
	return m_SimulationWait;
}

int Scene::GetMaxSimulationWait() {
	return MAX_CONVEX_HULL_SIMULATION_WAIT;
}

void Scene::SetConvexHullSimulationCompleted(bool completion) {
	m_ConvexHullSimulationCompleted = completion;
}

void Scene::SetLineSweepSimulationCompleted(bool completion) {
	m_LineSweepSimulationCompleted = completion;
}

bool Scene::GetLineSweepSimulationCompleted() const {
	return m_LineSweepSimulationCompleted;
}
