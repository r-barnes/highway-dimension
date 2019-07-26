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

/* Collects shortest paths with length (rFrom, rTo]. Only collect shortest
 * paths, which satisfy `predicate`. Assumes `start` always satisfies
 * `predicate`.
 */
template <typename Predicate>
std::unordered_set<std::vector<bool>>
collectShortestPaths(const DijkstraOutput& output, Predicate predicate,
                     int start, int64_t rFrom, int64_t rTo)
{
  const std::vector<int64_t>& distances = output.distances;
  const std::vector<std::vector<int>>& children = output.children;
  size_t vertexCnt = distances.size();
  std::unordered_map<int, std::vector<bool>> pathLookup;

  std::queue<int> Q;
  Q.push(start);
  std::vector<bool> startPath(vertexCnt, false);
  startPath[start] = true;
  pathLookup[start] = std::move(startPath);
  std::unordered_set<std::vector<bool>> paths;

  while (!Q.empty()) {
    const int cur = Q.front();
    Q.pop();

    for (const int neighbor: children[cur]) {
      if (!predicate(neighbor)) {
        continue;
      }

      const int64_t neighborDistance = distances[neighbor];
      if (neighborDistance > rTo) {
        continue;
      }

      std::vector<bool> path(pathLookup[cur]);
      path[neighbor] = true;
      pathLookup[neighbor] = std::move(path);

      if (rTo >= neighborDistance && neighborDistance > rFrom) {
        paths.insert(pathLookup[neighbor]);
      }

      Q.push(neighbor);
    }

    pathLookup.erase(cur);
  }

  return paths;
}

#endif /* __DIJKSTRA__ */
