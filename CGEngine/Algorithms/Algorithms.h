#pragma once
#include "Shapes/Polygon.h"

void FormConvexHull(Shape::Polygon polygon, Shape::Polygon& hull, int simulationSpeed);


/* 
Input : A set of line segments
Output : All intersections of the input line segments
*/
// Find predecessor of iterator in s.
std::vector<Shape::Event>::iterator pred(std::vector<Shape::Event>& s, std::vector<Shape::Event>::iterator it);

// Find successor of iterator in s
std::vector<Shape::Event>::iterator succ(std::vector<Shape::Event>& s, std::vector<Shape::Event>::iterator it);

bool onSegment(glm::vec2 p, glm::vec2 q, glm::vec2 r);

void LineSweepAlgorithm(std::vector<Shape::Line> listOfLine, Shape::Line& sweepLine, std::vector<Shape::Event>& render_Q,
	std::vector<Shape::Event>& render_T, std::vector<glm::vec2>& intersections, int simulationSpeed);

