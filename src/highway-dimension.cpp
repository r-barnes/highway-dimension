#include "highway-dimension.h"
#include "hitting-set.h"
#include "dijkstra.h"

#include <unordered_set>
#include <vector>
#include <utility>
#include <queue>
#include <algorithm>
#ifdef DEBUG
#include <cassert>
#endif

const int ratio = 2;

int
approximateHd(const Graph<WeightedEdge>& graph)
{
  #ifdef DEBUG
  fputs("Approximating highway dimension.\n", stderr);
  #endif
  const int vertexCnt = graph.vertexCnt;
  std::unordered_set<int> edgeWeights; // FIXME: actually no need to store these
  for (int u = 0; u < vertexCnt; ++u) {
    for (const WeightedEdge& e : graph[u]) {
      edgeWeights.insert(e.weight);
    }
  }
  #ifdef DEBUG
  fputs("Found distinct edge weights.\n", stderr);
  #endif

  std::vector<DijkstraOutput> dijkstraOutputs;
  for (int u = 0; u < vertexCnt; ++u) {
    dijkstraOutputs.push_back(std::move(dijkstra(graph, u)));
  }
  #ifdef DEBUG
  fputs("Ran Dijkstra from all vertices.\n", stderr);
  #endif

  int hd = 0;

  /* Apparently there are stations that can be traversed in 0 seconds.
   */
  edgeWeights.erase(0);
  for (const int w: edgeWeights) {
    #ifdef DEBUG
    fprintf(stderr, "Calculating highway dimension for weight %d.\n", w);
    #endif
    const int halfRadius = w / ratio;
    std::vector<int> localHubs(vertexCnt, 0);
    std::set<std::set<int>> paths;
    for (int u = 0; u < vertexCnt; ++u) {
      const std::set<std::set<int>> curPaths(
          collectShortestPaths(dijkstraOutputs[u], u, halfRadius, w));
      paths.insert(curPaths.begin(), curPaths.end());
    }
    #ifdef DEBUG
    fputs("Collected all shortest paths.\n", stderr);
    #endif

    Graph<int> hittingSetInstance(vertexCnt + paths.size());
    int pathIndex = vertexCnt;
    for (auto p = paths.begin(); p != paths.end(); ++p, ++pathIndex) {
      for (const int u : *p) {
        hittingSetInstance.addEdge(pathIndex, u);
        hittingSetInstance.addEdge(u, pathIndex);
      }
    }

    std::vector<int> hittingSetApx =
      approximateHittingSet(vertexCnt, hittingSetInstance);
    #ifdef DEBUG
    assert(hittingSetApx.size());
    #endif
    for (const int h : hittingSetApx) {
      for (int u = 0; u < vertexCnt; ++u) {
        const int distance = dijkstraOutputs[h].distances[u];
        if (halfRadius < distance && distance <= w) {
          ++localHubs[u];
        }
      }
    }

    hd = std::min(hd, *std::max_element(localHubs.begin(), localHubs.end()));
  }

  return hd;
}
