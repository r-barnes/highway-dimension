#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include "graph.h"

#include <vector>
#include <set>
#include <queue>
#include <unordered_map>
#include <cstdint>

extern const int voidParent;

/* Member `children` are children in shortest path tree.
 */
struct DijkstraOutput
{
  const std::vector<int64_t> distances;
  const std::vector<int> parents;
  const std::vector<std::vector<int>> children;

  DijkstraOutput(std::vector<int64_t>&& distances, std::vector<int>&& parents,
                 std::vector<std::vector<int>>&& children);
};

/* Returns a pair of distances and parents. Parents with value `voidParent`
 * mean unreachable aside from `start`.
 */
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
  const size_t vertexCnt = distances.size();
  std::unordered_map<int, std::set<int>> pathLookup;

  std::vector<bool> visited(vertexCnt, false);
  std::queue<int> Q;
  Q.push(start);
  pathLookup[start] = {start};
  std::set<std::set<int>> paths;

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
        std::set<int> path(pathLookup[cur]);
        path.insert(neighbor);
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
