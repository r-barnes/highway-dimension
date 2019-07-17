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

  Graph(int n);
};

#endif /* __GRAPH__ */
