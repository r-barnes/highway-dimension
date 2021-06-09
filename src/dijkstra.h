#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include "graph.h"

#include <cstdint>
#include <queue>
#include <set>
#include <unordered_map>
#include <vector>

/* Member `children` are children in shortest path tree.
 */
struct DijkstraOutput
{
  std::vector<int64_t> distances;
  std::vector<std::vector<int>> children;

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
std::set<std::set<int>>
collectShortestPaths(const DijkstraOutput& output, Predicate predicate,
                     int start, int64_t rFrom, int64_t rTo)
{
  const std::vector<int64_t>& distances = output.distances;
  const std::vector<std::vector<int>>& children = output.children;
  std::unordered_map<int, std::set<int>> pathLookup;

  std::queue<int> Q;
  Q.push(start);
  pathLookup[start] = {start};
  std::set<std::set<int>> paths;

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

      std::set<int> path(pathLookup[cur]);
      path.insert(neighbor);
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
