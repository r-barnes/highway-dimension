#include "highway-dimension.h"

#include <unordered_set>

int
approximateHd(Graph& graph)
{
  std::unordered_set<int> edgeWeights;
  for (int u = 0; u < graph.vertexCnt; ++u) {
    for (const Edge& e : graph[u]) {
      edgeWeights.insert(e.weight);
    }
  }

  for (const int r : edgeWeights) {
  }
}
