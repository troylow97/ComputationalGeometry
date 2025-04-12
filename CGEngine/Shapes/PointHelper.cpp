#include "pch.h"
#include "PointHelper.h"

int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r) {
	int val = static_cast<int>((q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y));

	if (val == 0)
		return 0;	// colinear

	return (val > 0) ? 1 : 2; 	// clock or counterclock wise
}

bool IsPointNearLineSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& P, float threshold) {
    glm::vec2 AB = B - A;
    glm::vec2 AP = P - A;
    float ab2 = glm::dot(AB, AB);
    float t = glm::dot(AP, AB) / ab2;

    // Clamp t between 0 and 1 to stay on segment
    t = glm::clamp(t, 0.0f, 1.0f);
    glm::vec2 closestPoint = A + t * AB;

    float distance = glm::length(P - closestPoint);
    return distance < threshold;
}

