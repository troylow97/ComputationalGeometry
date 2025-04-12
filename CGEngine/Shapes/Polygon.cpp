#include "pch.h"
#include "Polygon.h"

namespace Shape {
	Polygon::Polygon() :
		m_Vertices{}
	{}

	Polygon::Polygon(const glm::vec2& one) {
		m_Vertices.push_back(one);
	}

	Polygon::Polygon(const std::vector<glm::vec2>& dots) :
		m_Vertices{ dots }
	{}

	Polygon::Polygon(const Polygon& other)
	{
		m_Vertices = other.m_Vertices;
	}

	Polygon::Polygon(Polygon&& old) noexcept {
		m_Vertices.swap(old.m_Vertices);
	}

	Polygon::~Polygon() {}

	void Polygon::AddVertex(glm::vec2 new_vertex) {
		if (std::find(m_Vertices.begin(), m_Vertices.end(), new_vertex) != m_Vertices.end())
			return;

		m_Vertices.push_back(new_vertex);
	}

	void Polygon::RemoveVertex(glm::vec2 vertex) {
		m_Vertices.erase(std::remove_if(m_Vertices.begin(), m_Vertices.end(),
			[&](glm::vec2 vertexCompare) { return vertex == vertexCompare; }), m_Vertices.end());
	}

	void Polygon::RemoveVertex(int index) {
		m_Vertices.erase(m_Vertices.begin() + index);
	}

	size_t Polygon::getNumOfVertices() {
		return m_Vertices.size();
	}

	std::vector<glm::vec2> Polygon::getVertexList() {
		return m_Vertices;
	}

	std::vector<glm::ivec2> Polygon::getVertexListForRender() {
		std::vector<glm::ivec2> result;
		result.reserve(m_Vertices.size());
		for (auto& point : m_Vertices)
		{
			result.push_back(glm::ivec2(point.x, point.y));
		}
		return result;
	}

	glm::vec2 Polygon::getCenterPoint() {
		float sumOfAllVertex_X = 0.0f;
		float sumOfAllVertex_Y = 0.0f;
		for (auto& vertex : m_Vertices)
		{
			sumOfAllVertex_X += vertex.x;
			sumOfAllVertex_Y += vertex.y;
		}
		return glm::vec2{ sumOfAllVertex_X / m_Vertices.size(), sumOfAllVertex_Y / m_Vertices.size() };
	}

	std::vector<Shape::Line> Shape::Polygon::GetEdges(bool closeLoop) const {
		std::vector<Shape::Line> edges;

		if (m_Vertices.size() < 2)
			return edges;

		for (size_t i = 0; i < m_Vertices.size() - 1; ++i) {
			edges.emplace_back(m_Vertices[i], m_Vertices[i + 1]);
		}

		if (closeLoop) {
			edges.emplace_back(m_Vertices.back(), m_Vertices.front());
		}

		return edges;
	}



	void Polygon::clearVertices() {
		m_Vertices.clear();
	}

	Polygon& Polygon::operator=(const Polygon& other) {
		m_Vertices = other.m_Vertices;
		return *this;
	}

	Polygon& Polygon::operator += (glm::vec2 new_vertex) {
		AddVertex(new_vertex);
		return *this;
	}

	glm::vec2 Polygon::operator[](int a) {
		try
		{
			return m_Vertices[a];
		}
		catch (std::exception e)
		{
			throw std::out_of_range("The Polygon::operator [] out of range");
		}
	}

	void Polygon::printListOfVertices() {
		if (m_Vertices.empty())
			std::cout << "There is no polygon" << std::endl;
		else
		{
			std::cout << "List of Vertices: ";
			for (int i = 0; i < (m_Vertices.size() - 1); i++) {
				std::cout << "{" << m_Vertices[i].x << "," << m_Vertices[i].y << "}, ";
			}
			std::cout << "{" << m_Vertices[(m_Vertices.size() - 1)].x << "," << m_Vertices[(m_Vertices.size() - 1)].y << "}" << std::endl;
		}
	}
}
