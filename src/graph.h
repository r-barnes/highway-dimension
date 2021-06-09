#ifndef __GRAPH__
#define __GRAPH__

#include <cstdint>
#include <vector>

struct WeightedEdge
{
  const int to;
  const int64_t weight;
};

template <typename EdgeType>
class Graph {
public:
  Graph() = default;
  Graph(int n);
  Graph(const Graph<EdgeType>& src);
  Graph(Graph<EdgeType>&& src);

  std::vector<EdgeType>& operator[](size_t index);
  const std::vector<EdgeType>& operator[](size_t index) const;

  void addEdge(int from, const EdgeType& edge);
  void addEdge(int from, EdgeType&& edge);

  int64_t vertex_count() const;
  int64_t edge_count() const;

protected:
  int64_t m_edge_count = 0;
  std::vector<std::vector<EdgeType>> neighborList;

private:
  void make_enough_vertices(const int vertex_id1, const EdgeType &edge);
};

template <typename EdgeType>
Graph<EdgeType>::Graph(int n) : neighborList(n)
{
}

template <typename EdgeType>
Graph<EdgeType>::Graph(const Graph<EdgeType>& src)
  : m_edge_count(src.m_edge_count), neighborList(src.neighborList)
{
}

template <typename EdgeType>
Graph<EdgeType>::Graph(Graph<EdgeType>&& src)
  : m_edge_count(src.m_edge_count), neighborList(std::move(src.neighborList))
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
  make_enough_vertices(from, edge); 
  neighborList[from].push_back(edge);
  m_edge_count++;
}

template <typename EdgeType>
void
Graph<EdgeType>::addEdge(int from, EdgeType&& edge)
{
  make_enough_vertices(from, edge); 
  neighborList[from].push_back(std::move(edge));
  m_edge_count++;
}

template <typename EdgeType>
int64_t
Graph<EdgeType>::vertex_count() const
{
  return neighborList.size();
}

template <typename EdgeType>
int64_t
Graph<EdgeType>::edge_count() const
{
  return m_edge_count;
}

template <>
void inline 
Graph<WeightedEdge>::make_enough_vertices(const int vertex_id1, const WeightedEdge &edge) 
{
  const size_t max_id = 1 + std::max(vertex_id1, edge.to); //Use max to avoid double resize
  if(max_id>=neighborList.size()){
    neighborList.resize(max_id);
  }
}

template <>
void inline
Graph<int>::make_enough_vertices(const int vertex_id1, const int &vertex_id2) 
{
  const size_t max_id = 1 + std::max(vertex_id1, vertex_id2); //Use max to avoid double resize
  if(max_id>=neighborList.size()){
    neighborList.resize(max_id);
  }
}

#endif /* __GRAPH__ */
