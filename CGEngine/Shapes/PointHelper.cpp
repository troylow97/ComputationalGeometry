#include "pch.h"
#include "PointHelper.h"

int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r) {
	int val = static_cast<int>((q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y));

	if (val == 0)
		return 0;	// colinear

	return (val > 0) ? 1 : 2; 	// clock or counterclock wise
}

