#include "highway-dimension.h"
#include "hitting-set.h"
#include "dijkstra.h"

#include <unordered_set>
#include <vector>
#include <algorithm>
#ifdef DEBUG
#include <cinttypes>
#include <cassert>
#include <cstdio>
#endif

std::unordered_set<int64_t>
collectDistinctWeights(const Graph<WeightedEdge>& graph)
{
  std::unordered_set<int64_t> result;
  const int vertexCnt = graph.vertexCnt;
  for (int u = 0; u < vertexCnt; ++u) {
    for (const WeightedEdge& e : graph[u]) {
      result.insert(e.weight);
    }
  }

  /* Apparently there are stations that can be traversed in 0 seconds.
   */
  result.erase(0);
  return result;
}

std::vector<DijkstraOutput>
dijkstraFromAllVertices(const Graph<WeightedEdge>& graph)
{
  const int vertexCnt = graph.vertexCnt;
  std::vector<DijkstraOutput> result;
  for (int u = 0; u < vertexCnt; ++u) {
    result.push_back(dijkstra(graph, u));
  }
  return result;
}

/* Imho this does not work correctly.
 */
[[deprecated]]
int
approximateSparseSPC(const Graph<WeightedEdge>& graph)
{
  static const int64_t ratio = 2;
  #ifdef DEBUG
  fputs("Approximating sparse shortest path cover.\n", stderr);
  #endif
  const int vertexCnt = graph.vertexCnt;
  const std::unordered_set<int64_t> edgeWeights = collectDistinctWeights(graph);
  #ifdef DEBUG
  fputs("Found distinct edge weights.\n", stderr);
  #endif

  const std::vector<DijkstraOutput> dijkstraOutputs =
    dijkstraFromAllVertices(graph);
  #ifdef DEBUG
  fputs("Ran Dijkstra from all vertices.\n", stderr);
  #endif

  int hd = 0;

  auto predicate = [](__attribute__((unused))int u)
  {
    return true;
  };
  for (const int64_t w: edgeWeights) {
    #ifdef DEBUG
    fprintf(stderr, "Calculating highway dimension for weight %" PRId64
                    ".\n", w);
    #endif
    const int64_t halfRadius = w / ratio;
    std::vector<int> localHubs(vertexCnt, 0);
    std::set<std::set<int>> paths;
    for (int u = 0; u < vertexCnt; ++u) {
      const std::set<std::set<int>> curPaths(
          collectShortestPaths(dijkstraOutputs[u], predicate, u, halfRadius, w));
      paths.insert(curPaths.begin(), curPaths.end());
    }
    #ifdef DEBUG
    fputs("Collected all shortest paths.\n", stderr);
    #endif

    Graph<int> hittingSetInstance(vertexCnt + paths.size());
    int pathIndex = vertexCnt;
    for (auto p = paths.cbegin(); p != paths.cend(); ++p, ++pathIndex) {
      for (const int u : *p) {
        hittingSetInstance.addEdge(pathIndex, u);
        hittingSetInstance.addEdge(u, pathIndex);
      }
    }
    std::vector<int> hittingSetApx =
      approximateHittingSet(vertexCnt, hittingSetInstance);
    #ifdef DEBUG
    assert(hittingSetApx.size() > 0);
    #endif

    for (const int h : hittingSetApx) {
      for (int u = 0; u < vertexCnt; ++u) {
        const int64_t distance = dijkstraOutputs[h].distances[u];
        if (distance <= w) {
          ++localHubs[u];
        }
      }
    }

    int curH = *std::max_element(localHubs.begin(), localHubs.end());
    #ifdef DEBUG
    if (curH < 1) {
      fputs("! Hub density is 0!", stderr);
    }
    #endif
    hd = std::max(hd, curH);
  }

  return hd;
}

int
approximateHd(const Graph<WeightedEdge>& graph)
{
  #ifdef DEBUG
  fputs("Approximating highway dimension.\n", stderr);
  #endif
  static const int64_t ratio = 4;
  const int vertexCnt = graph.vertexCnt;
  // FIXME: no need to store these
  const std::unordered_set<int64_t> edgeWeights = collectDistinctWeights(graph);
  #ifdef DEBUG
  fputs("Collected distinct edge weights.\n", stderr);
  #endif
  const std::vector<DijkstraOutput> dijkstraOutputs =
    dijkstraFromAllVertices(graph);
  #ifdef DEBUG
  fputs("Ran Dijkstra from all vertices.\n", stderr);
  #endif
  size_t hd = 0;

  for (const int64_t w: edgeWeights) {
    const int64_t quarterW = w / ratio;
    #ifdef DEBUG
    fprintf(stderr, "Calculating highway dimension for range (%" PRId64
                    ", %" PRId64 "].\n",
            quarterW, w);
    #endif
    for (int u = 0; u < vertexCnt; ++u) {
      #ifdef DEBUG
      fprintf(stderr, "Vertex %d.\n", u);
      #endif
      const DijkstraOutput& myDijkstraOutput = dijkstraOutputs[u];

      std::unordered_set<int> ball;
      for (int v = 0; v < vertexCnt; ++v) {
        if (myDijkstraOutput.distances[v] <= w) {
          ball.insert(v);
        }
      }
      #ifdef DEBUG
      fputs("Found ball.\n", stderr);
      #endif

      std::set<std::set<int>> shortestPaths;
      auto predicate = [&ball = std::as_const(ball)](int u)
      {
        return ball.count(u);
      };
      for (int v = 0; v < vertexCnt; ++v) {
        const std::set<std::set<int>> curPaths(
            collectShortestPaths(myDijkstraOutput, predicate, v, quarterW, w));
        shortestPaths.insert(curPaths.begin(), curPaths.end());
      }
      #ifdef DEBUG
      fputs("Collected shortest paths.\n", stderr);
      #endif

      Graph<int> hittingSetInstance(vertexCnt + shortestPaths.size());
      int pathIndex = vertexCnt;
      for (auto p = shortestPaths.cbegin(); p != shortestPaths.cend();
           ++p, ++pathIndex) {
        for (const int u: *p) {
          hittingSetInstance.addEdge(pathIndex, u);
          hittingSetInstance.addEdge(u, pathIndex);
        }
      }
      std::vector<int> hittingSet =
        approximateHittingSet(vertexCnt, hittingSetInstance);
      #ifdef DEBUG
      fputs("Solved hitting set.\n", stderr);
      #endif
      hd = std::max(hd, hittingSet.size());
    }
  }

  return hd;
}
