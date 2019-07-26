#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include "graph.h"

#include <vector>
#include <unordered_set>
#include <queue>
#include <unordered_map>
#include <cstdint>

/* Member `children` are children in shortest path tree.
 */
struct DijkstraOutput
{
  const std::vector<int64_t> distances;
  const std::vector<std::vector<int>> children;

  DijkstraOutput(std::vector<int64_t>&& distances,
                 std::vector<std::vector<int>>&& children);
};

DijkstraOutput
dijkstra(const Graph<WeightedEdge>& graph, const int start);

/* Collects shortest paths with length (rFrom, rTo]. A path is represented as
 * a bitset.
 *
 * Only collects shortest paths, which satisfy `predicate`. Assumes `start`
 * always satisfies `predicate`.
 */
template <typename Predicate>
std::unordered_set<std::vector<bool>>
collectShortestPaths(const DijkstraOutput& output, Predicate predicate,
                     int start, int64_t rFrom, int64_t rTo)
{
  const std::vector<int64_t>& distances = output.distances;
  const std::vector<std::vector<int>>& children = output.children;
  const size_t vertexCnt = distances.size();
  std::unordered_map<int, std::vector<bool>> pathLookup;

  std::vector<bool> visited(vertexCnt, false);
  std::queue<int> Q;
  Q.push(start);
  std::vector<bool> startBitset(vertexCnt, false);
  startBitset[start] = 1;
  pathLookup[start] = std::move(startBitset);
  std::unordered_set<std::vector<bool>> paths;

  while (!Q.empty()) {
    const int cur = Q.front();
    Q.pop();
    visited[cur] = true;

    for (const int neighbor: children[cur]) {
      if (visited[neighbor]) {
        continue;
      }
      if (!predicate(neighbor)) {
        continue;
      }
      const int64_t neighborDistance = distances[neighbor];
      if (neighborDistance > rTo) {
        continue;
      }

      if (rTo >= neighborDistance && neighborDistance > rFrom) {
        std::vector<bool> path(pathLookup[cur]);
        path[neighbor] = true;
        pathLookup[neighbor] = path;
        paths.insert(path);
      }

      pathLookup.erase(cur);
      Q.push(neighbor);
    }
  }

  return paths;
}

#endif /* __DIJKSTRA__ */
