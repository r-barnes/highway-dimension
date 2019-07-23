#include "highway-dimension.h"
#include "hitting-set.h"
#include "dijkstra.h"

#include <set>
#include <unordered_set>
#include <vector>
#include <utility>
#include <queue>

const int ratio = 2;

/* Collects shortest paths for a single start.
 */
std::set<std::set<int>>
collectShortestPaths(const DijkstraOutput& output, int start, int radius)
{
  const std::vector<int>& distances = output.distances;
  const std::vector<int>& parents = output.parents;
  const std::vector<std::vector<int>>& next = output.next;
  const int vertexCnt = distances.size();

  std::vector<bool> visited(vertexCnt, false);
  std::queue<int> Q;
  Q.push(start);
  std::set<std::set<int>> paths;

  while (!Q.empty()) {
    const int cur = Q.front();
    Q.pop();
    visited[cur] = true;

    for (const int neighbor: next[cur]) {
      if (visited[neighbor]) {
        continue;
      }
      const int neighborDistance = distances[neighbor];
      if (neighborDistance > radius * ratio) {
        continue;
      }

      // FIXME: if parent is admissible, then just add me to his path
      if (neighborDistance > radius) {
        std::set<int> path;
        for (int endOfPath = neighbor; endOfPath != voidParent;
             endOfPath = parents[endOfPath]) {
          path.insert(endOfPath);
        }
        paths.insert(path);
      }

      Q.push(neighbor);
    }
  }

  return paths;
}

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

  size_t hd = 0;

  for (const int w: edgeWeights) {
    std::set<std::set<int>> paths;
    for (int u = 0; u < vertexCnt; ++u) {
      const std::set<std::set<int>> curPaths(
          collectShortestPaths(dijkstraOutputs[u], u, (w + ratio - 1) / ratio));
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

    std::vector<int> hittingSetApx = approximateHittingSet(vertexCnt,
                                                           hittingSetInstance);
    hd = std::max(hd, hittingSetApx.size());
  }

  return hd;
}
