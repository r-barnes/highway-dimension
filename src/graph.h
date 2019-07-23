#ifndef __GRAPH__
#define __GRAPH__

#include <vector>
#include <utility>

struct Edge
{
  const int to;
};

struct WeightedEdge
{
  const int to;
  const int weight;
};

template <typename EdgeType>
class Graph {
public:
  Graph(int n);
  Graph(const Graph<EdgeType>& src);
  Graph(Graph<EdgeType>&& src);

  std::vector<EdgeType>& operator[](size_t index);
  const std::vector<EdgeType>& operator[](size_t index) const;

  void addEdge(int from, const EdgeType& edge);
  void addEdge(int from, EdgeType&& edge);

  const int vertexCnt;

protected:
  std::vector<std::vector<EdgeType>> neighborList;
};

template <typename EdgeType>
Graph<EdgeType>::Graph(int n) : vertexCnt(n), neighborList(vertexCnt)
{
}

template <typename EdgeType>
Graph<EdgeType>::Graph(const Graph<EdgeType>& src)
  : vertexCnt(src.vertexCnt), neighborList(src.neighborList)
{
}

template <typename EdgeType>
Graph<EdgeType>::Graph(Graph<EdgeType>&& src)
  : vertexCnt(src.vertexCnt), neighborList(std::move(src.neighborList))
{
}

template <typename EdgeType>
std::vector<EdgeType>&
Graph<EdgeType>::operator[](size_t index)
{
  return neighborList[index];
}


template <typename EdgeType>
const std::vector<EdgeType>&
Graph<EdgeType>::operator[](size_t index) const
{
  return neighborList[index];
}

template <typename EdgeType>
void
Graph<EdgeType>::addEdge(int from, const EdgeType& edge)
{
  neighborList[from].push_back(edge);
}

template <typename EdgeType>
void
Graph<EdgeType>::addEdge(int from, EdgeType&& edge)
{
  neighborList[from].push_back(std::move(edge));
}

#endif /* __GRAPH__ */
