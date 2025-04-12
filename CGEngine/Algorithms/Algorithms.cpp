#include "pch.h"
#include "Shapes/PointHelper.h"
#include "Algorithms.h"
#include <thread>
#include <chrono> 
#include <unordered_set>
#include <set>
#include <map>
void FormConvexHull(Shape::Polygon polygon, Shape::Polygon& hull, int simulationSpeed) {
	size_t size = polygon.getNumOfVertices();

	// There must be at least 3 points
	if (polygon.getVertexList().size() < 3)
		return;

	// Initialize Result
	//std::vector<glm::vec2> hull;

	// Find the leftmost point
	int l = 0;
	for (int i = 1; i < size; i++) {
		if (polygon.getVertexList()[i].x < polygon.getVertexList()[l].x)
			l = i;
	}

	// Start from leftmost point, keep moving counterclockwise
	// until reach the start point again.  This loop runs O(h)
	// times where h is number of points in result or output.
	int p = l, q;
	do {
		// Add current point to result
		hull.AddVertex(polygon.getVertexList()[p]);
		std::this_thread::sleep_for(std::chrono::milliseconds(simulationSpeed));

		// Search for a point 'q' such that orientation(p, x,
		// q) is counterclockwise for all points 'x'. The idea
		// is to keep track of last visited most counterclock-
		// wise point in q. If any point 'i' is more counterclock-
		// wise than q, then update q.
		q = (p + 1) % size;

		for (int i = 0; i < size; i++) {
			// If i is more counterclockwise than current q, then
			// update q
			if (orientation(polygon.getVertexList()[p], polygon.getVertexList()[i], polygon.getVertexList()[q]) == 2)
				q = i;
		}

		// Now q is the most counterclockwise with respect to p
		// Set p as q for next iteration, so that q is added to
		// result 'hull'
		p = q;
	} while (p != l);  // While we don't come to first point
}

std::vector<Shape::Event>::iterator pred(std::vector<Shape::Event>& s, std::vector<Shape::Event>::iterator it) {
	return (it == s.begin() ? s.end() : --it);
}

std::vector<Shape::Event>::iterator succ(std::vector<Shape::Event>& s, std::vector<Shape::Event>::iterator it) {
	(void)s;
	return ++it;
}

bool onSegment(glm::vec2 p, glm::vec2 q, glm::vec2 r) {
	if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
		q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
		return true;

	return false;
}

bool doIntersect(Shape::Line s1, Shape::Line s2) {
	glm::vec2 p1 = s1.GetTop(), q1 = s1.GetBottom(), p2 = s2.GetTop(), q2 = s2.GetBottom();

	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;

	// Special Cases
	// p1, q1 and p2 are collinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1)) return true;

	// p1, q1 and q2 are collinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1)) return true;

	// p2, q2 and p1 are collinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2)) return true;

	// p2, q2 and q1 are collinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2)) return true;

	return false; // Doesn't fall in any of the above cases
}

glm::vec2 lineLineIntersection(glm::vec2 A, glm::vec2 B, glm::vec2 C, glm::vec2 D) {
	// Line AB represented as a1x + b1y = c1
	float a1 = B.y - A.y;
	float b1 = A.x - B.x;
	float c1 = a1 * (A.x) + b1 * (A.y);

	// Line CD represented as a2x + b2y = c2
	float a2 = D.y - C.y;
	float b2 = C.x - D.x;
	float c2 = a2 * (C.x) + b2 * (C.y);

	float determinant = a1 * b2 - a2 * b1;

	if (determinant == 0)
	{
		// The lines are parallel. This is simplified
		// by returning a pair of FLT_MAX
		return { FLT_MAX, FLT_MAX };
	}
	else
	{
		float x = (b2 * c1 - b1 * c2) / determinant;
		float y = (a1 * c2 - a2 * c1) / determinant;
		return { x, y };
	}
}


/*
In a line sweep algorithm, Q (event queue) stores events (like segment endpoints and intersections) ordered by their x-coordinate, 
while T (status structure) stores active segments, ordered by their y-coordinate along the sweep line. 
Essentially, Q determines when an event happens, and T maintains the state of the sweep line as it progresses.
*/
void LineSweepAlgorithm(std::vector<Shape::Line> listOfLine, Shape::Line& sweepLine, std::vector<Shape::Event>& render_Q,
	std::vector<Shape::Event>& render_T, std::vector<glm::vec2>& intersections, int simulationSpeed)
{
	//To store all possible combination index of intersect
	std::map<std::pair<int, int>, glm::vec2> mp;

	//To store event 
	std::vector<Shape::Event> Q;
	std::vector<Shape::Event> T;

	//Push all the event into Q without the intersect 
	for (int i = 0; i < listOfLine.size(); ++i) {
		listOfLine[i].SetID(i);
		auto top = listOfLine[i].GetTop();
		auto bottom = listOfLine[i].GetBottom();
		Q.push_back(Shape::Event(top, true, i));
		Q.push_back(Shape::Event(bottom, false, i));
	}

	//Sort all the event Q by the y-coordinate than x-coordinate
	std::sort(Q.begin(), Q.end(), [](Shape::Event& e1, Shape::Event& e2) {return e1 < e2; });
	// We pass by reference for Q back to scene.cpp for rendering
	render_Q = Q;
	std::this_thread::sleep_for(std::chrono::milliseconds(simulationSpeed));

	//While Q is active/ not empty
	while (Q.size() != 0) {
		//Get current Q, and remove it at the end
		Shape::Event curr = *Q.begin();
		int index = curr.indexID;

		//If event is intersect
		if (curr.isIntersect) {
			auto index1 = curr.indexID;
			auto index2 = curr.indexID2;

			int in1 = 0;
			int in2 = 0;
			for (int i = 0; i < T.size(); i++) {
				if (T[i].indexID == index1)
					in1 = i;
				if (T[i].indexID == index2)
					in2 = i;
			}

			//Swaping of the T
			auto temp = T[in1];
			T[in1] = T[in2];
			T[in2] = temp;

			//Checking intersection
			std::vector<Shape::Event>::iterator itprev = T.begin();
			std::vector<Shape::Event>::iterator itnext = T.begin();
			if (in1 < in2) {
				itprev = pred(T, T.begin() + in1);
				itnext = succ(T, T.begin() + in2);

				if (itnext != T.end() && itnext->indexID != T[in2].indexID && doIntersect(listOfLine[itnext->indexID], listOfLine[T[in2].indexID])) {
					if (mp.find({ T[in2].indexID,itnext->indexID }) == mp.end() && mp.find({ itnext->indexID,index }) == mp.end()) {
						auto intersect = lineLineIntersection(listOfLine[T[in2].indexID].GetTop(), listOfLine[T[in2].indexID].GetBottom(), listOfLine[itnext->indexID].GetTop(), listOfLine[itnext->indexID].GetBottom());
						Q.push_back(Shape::Event(intersect, false, T[in2].indexID, true, itnext->indexID));
						mp.insert({ {T[in2].indexID,itnext->indexID }, intersect });
					}
				}

				if (itprev != T.end() && itprev->indexID != T[in1].indexID && doIntersect(listOfLine[itprev->indexID], listOfLine[T[in1].indexID])) {
					if (mp.find({ T[in1].indexID,itprev->indexID }) == mp.end() && mp.find({ itprev->indexID,index }) == mp.end()) {
						auto intersect = lineLineIntersection(listOfLine[T[in1].indexID].GetTop(), listOfLine[T[in1].indexID].GetBottom(), listOfLine[itprev->indexID].GetTop(), listOfLine[itprev->indexID].GetBottom());
						Q.push_back(Shape::Event(intersect, false, T[in1].indexID, true, itprev->indexID));
						mp.insert({ {T[in1].indexID,itprev->indexID }, intersect });
					}
				}
			}
			else {
				itprev = pred(T, T.begin() + in2);
				itnext = succ(T, T.begin() + in1);

				if (itnext != T.end() && itnext->indexID != T[in1].indexID && doIntersect(listOfLine[itnext->indexID], listOfLine[T[in1].indexID])) {
					if (mp.find({ T[in1].indexID,itnext->indexID }) == mp.end() && mp.find({ itnext->indexID,index }) == mp.end()) {
						auto intersect = lineLineIntersection(listOfLine[T[in1].indexID].GetTop(), listOfLine[T[in1].indexID].GetBottom(), listOfLine[itnext->indexID].GetTop(), listOfLine[itnext->indexID].GetBottom());
						Q.push_back(Shape::Event(intersect, false, T[in1].indexID, true, itnext->indexID));
						mp.insert({ {T[in1].indexID,itnext->indexID }, intersect });
					}
				}

				if (itprev != T.end() && itprev->indexID != T[in2].indexID && doIntersect(listOfLine[itprev->indexID], listOfLine[T[in2].indexID])) {
					if (mp.find({ T[in2].indexID,itprev->indexID }) == mp.end() && mp.find({ itprev->indexID,index }) == mp.end()) {
						auto intersect = lineLineIntersection(listOfLine[T[in2].indexID].GetTop(), listOfLine[T[in2].indexID].GetBottom(), listOfLine[itprev->indexID].GetTop(), listOfLine[itprev->indexID].GetBottom());
						Q.push_back(Shape::Event(intersect, false, T[in2].indexID, true, itprev->indexID));
						mp.insert({ {T[in2].indexID,itprev->indexID }, intersect });
					}
				}
			}

		}
		//If event is a point of the line at the top
		else if (curr.isTop) {
			//Find the position to slot in the current event
			int i = 0;
			for (i = 0; i < T.size(); i++)
				if (T[i].point.x > curr.point.x)
					break;

			T.insert(T.begin() + i, curr);
			auto c = T.begin() + i;

			//Get the next and previous of the current T
			auto prev = pred(T, c);
			auto next = succ(T, c);

			// Check current T intersect with the next T
			if (next != T.end() && next->indexID != index && doIntersect(listOfLine[next->indexID], listOfLine[index])) {
				if (mp.find({ index,next->indexID }) == mp.end() && mp.find({ next->indexID,index }) == mp.end()) {
					auto intersect = lineLineIntersection(listOfLine[index].GetTop(), listOfLine[index].GetBottom(), listOfLine[next->indexID].GetTop(), listOfLine[next->indexID].GetBottom());
					Q.push_back(Shape::Event(intersect, false, index, true, next->indexID));
					mp.insert({ {index,next->indexID }, intersect });
				}
			}

			// Check current T intersect with the prev T
			if (prev != T.end() && prev->indexID != index && doIntersect(listOfLine[prev->indexID], listOfLine[index])) {
				if (mp.find({ index,prev->indexID }) == mp.end() && mp.find({ prev->indexID,index }) == mp.end()) {
					auto intersect = lineLineIntersection(listOfLine[index].GetTop(), listOfLine[index].GetBottom(), listOfLine[prev->indexID].GetTop(), listOfLine[prev->indexID].GetBottom());
					Q.push_back(Shape::Event(intersect, false, index, true, prev->indexID));
					mp.insert({ { index,prev->indexID }, intersect });
				}
			}
		}
		//If event is a point of the line at the bottom
		else {
			//Find the position to slot in the current event
			int i = 0;
			for (i = 0; i < T.size(); i++)
				if (T[i].indexID == curr.indexID)
					break;

			auto c = T.begin() + i;
			auto prev = pred(T, c);
			auto next = succ(T, c);

			// Check prev T intersect with the next T
			if (prev != T.end() && next != T.end() && prev->indexID != next->indexID) {
				if (doIntersect(listOfLine[prev->indexID], listOfLine[next->indexID])) {
					if (mp.find({ prev->indexID,next->indexID }) == mp.end() && mp.find({ next->indexID,prev->indexID }) == mp.end()) {
						auto intersect = lineLineIntersection(listOfLine[next->indexID].GetTop(), listOfLine[next->indexID].GetBottom(), listOfLine[prev->indexID].GetTop(), listOfLine[prev->indexID].GetBottom());
						Q.push_back(Shape::Event(intersect, false, prev->indexID, true, prev->indexID));
						mp.insert({ { prev->indexID,next->indexID }, intersect });
					}
				}
			}

			//Remove the current from T
			T.erase(c);
		}

		intersections.clear();
		for (auto& pr : mp) {
			//std::cout << "Line " << pr.first.first << " and Line " << pr.first.second << " : " << pr.second.x << " | " << pr.second.y << "\n";
			intersections.push_back({ pr.second.x , pr.second.y });
		}

		// Next, we set the y coordinates of the sweep line to the current event point
		sweepLine.GetVerticesStart().y = curr.point.y;
		sweepLine.GetVerticesEnd().y = curr.point.y;
		std::this_thread::sleep_for(std::chrono::milliseconds(simulationSpeed));

		// We pass by reference for T back to scene.cpp for rendering
		render_T = T;
		std::this_thread::sleep_for(std::chrono::milliseconds(simulationSpeed));

		// We pass by reference for Q and T back to scene.cpp for rendering
		// After this iteration, we erase from the top of the queue
		Q.erase(Q.begin());
		std::sort(Q.begin(), Q.end(), [](Shape::Event& e1, Shape::Event& e2) {return e1 < e2; });

		// We pass by reference for Q back to scene.cpp for rendering
		render_Q = Q;
		std::this_thread::sleep_for(std::chrono::milliseconds(simulationSpeed));
	}
}

