#pragma once

int orientation(glm::vec2 p, glm::vec2 q, glm::vec2 r);
bool IsPointNearLineSegment(const glm::vec2& A, const glm::vec2& B, const glm::vec2& P, float threshold = 5.0f);