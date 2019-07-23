#include "highway-dimension.h"

#include <unordered_set>

int
approximateHd(Graph<WeightedEdge>& graph)
{
  std::unordered_set<int> edgeWeights;
  for (int u = 0; u < graph.vertexCnt; ++u) {
    for (const WeightedEdge& e : graph[u]) {
      edgeWeights.insert(e.weight);
    }
  }

  return 0;
}
