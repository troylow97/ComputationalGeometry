#pragma once

namespace  Shape {
	class Polygon
	{
	private:
		std::vector<glm::vec2> m_Vertices;
	public:
		//Constructors
		Polygon();
		Polygon(const glm::vec2& one);
		Polygon(const std::vector<glm::vec2>& other);
		Polygon(const Polygon& old);
		Polygon(Polygon&& old) noexcept;
		~Polygon();

		//Utils
		void AddVertex(glm::vec2 new_vertex);
		void RemoveVertex(glm::vec2 vertex);
		void RemoveVertex(int index);

		//GetterSetter
		size_t getNumOfVertices();
		std::vector<glm::vec2> getVertexList();
		std::vector<glm::ivec2> getVertexListForRender();
		glm::vec2 getCenterPoint();
		void clearVertices();

		//Operators
		Polygon& operator=(const Polygon& other);
		Polygon& operator+=(glm::vec2 new_point);
		glm::vec2 operator[] (int index);

		//Stream
		void printListOfVertices();
	};

	class Line
	{
	private:
		int m_ID;
		glm::vec2 m_VerticesStart;
		glm::vec2 m_VerticesEnd;
	public:
		Line() :m_ID{ 0 }, m_VerticesStart({ 0,0 }), m_VerticesEnd({ 0,0 })
		{}

		Line(const glm::vec2& input1, const glm::vec2& input2) : m_ID{ 0 },m_VerticesStart(input1), m_VerticesEnd(input2)
		{}

		Line(const Line& other) : m_ID{ other.m_ID },m_VerticesStart(other.m_VerticesStart), m_VerticesEnd(other.m_VerticesEnd)
		{}

		void SetID(const int newID)
		{
			m_ID = newID;
		}
		int GetID() const
		{
			return m_ID;
		}

		glm::vec2& GetVerticesStart()
		{
			return m_VerticesStart;
		}

		glm::vec2& GetVerticesEnd()
		{
			return m_VerticesEnd;
		}

		glm::vec2 GetTop() const
		{
			if (Mathf::SameFloat(m_VerticesStart.y, m_VerticesEnd.y))
			{
				if (m_VerticesStart.x > m_VerticesEnd.x)
					return m_VerticesEnd;
				else
					return m_VerticesStart;
			}

			if (m_VerticesStart.y > m_VerticesEnd.y)
				return m_VerticesStart;
			else
				return m_VerticesEnd;
		}

		glm::vec2 GetBottom() const
		{
			if (Mathf::SameFloat(m_VerticesStart.y, m_VerticesEnd.y))
			{
				if (m_VerticesStart.x > m_VerticesEnd.x)
					return m_VerticesStart;
				else
					return m_VerticesEnd;
			}

			if (m_VerticesStart.y > m_VerticesEnd.y)
				return m_VerticesEnd;
			else
				return m_VerticesStart;
		}

		bool operator==(const Shape::Line& rhs) const
		{
			return (GetTop() == rhs.GetTop() && GetBottom() == rhs.GetBottom());
		}
	};

	struct Event {
		glm::vec2 point;
		bool isTop;
		int indexID;

		bool isIntersect;
		int indexID2;

		Event(float x, float y, bool l, int i) : point(x,y), isTop(l), indexID(i), isIntersect(false), indexID2(0) {}
		Event(glm::vec2 pos, bool l, int i) : point(pos), isTop(l), indexID(i), isIntersect(false), indexID2(0) {}

		Event(glm::vec2 pos, bool l, int i, bool e, int ii) : point(pos), isTop(l), indexID(i), isIntersect(e),indexID2(ii) {}

		bool operator<(const Event& e) const 
		{
			if (point.x == e.point.x && point.y == e.point.y && isTop)
				return (isTop != e.isTop ? true : false);
			if (point.x == e.point.x && point.y == e.point.y && isIntersect)
				return (e.isTop != true ? true : false);
			if (point.y == e.point.y)
				return point.x < e.point.x;

			return point.y > e.point.y;
		}
	};
}
