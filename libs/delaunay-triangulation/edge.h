#ifndef H_EDGE
#define H_EDGE

#include "vector2.h"

namespace delaunay {

template <class T>
class Edge
{
	public:
		using VertexType = Vector2<T>;
		
		Edge(const VertexType &p1, const VertexType &p2, T w=-1) : p1(p1), p2(p2), weight(w) {};
		Edge(const Edge &e) : p1(e.p1), p2(e.p2), weight(e.weight), isBad(false) {};
		Edge(): p1(0,0), p2(0,0), weight(0), isBad(false) {}

    Edge& operator=(const Edge& rhs) {
      p1 = rhs.p1;
      p2 = rhs.p2;
      weight = rhs.weight;
      isBad = rhs.isBad;
      return *this;
    }

		VertexType p1;
		VertexType p2;
        T weight=0;

		bool isBad;
};

template <class T>
inline std::ostream &operator << (std::ostream &str, Edge<T> const &e)
{
	return str << "Edge " << e.p1 << ", " << e.p2;
}

template <class T>
inline bool operator == (const Edge<T> & e1, const Edge<T> & e2)
{
	return 	(e1.p1 == e2.p1 && e1.p2 == e2.p2) ||
			(e1.p1 == e2.p2 && e1.p2 == e2.p1);
}

}

#endif 

