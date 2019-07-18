#ifndef __GRAPH__
#define __GRAPH__

#include <vector>

struct Edge {
  const int to;
  const int weight;
};

struct Graph {
  const int vertexCnt;
  std::vector<std::vector<Edge>> neighborList;

  std::vector<Edge>& operator[](size_t index);
  const std::vector<Edge>& operator[](size_t index) const;

  Graph(int n);
};

#endif /* __GRAPH__ */
