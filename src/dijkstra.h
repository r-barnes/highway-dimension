#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include "graph.h"

#include <utility>
#include <vector>

extern const int voidParent;

/* Member `children` are children in shortest path tree.
 */
struct DijkstraOutput
{
  const std::vector<int> distances;
  const std::vector<int> parents;
  const std::vector<std::vector<int>> children;

  DijkstraOutput(std::vector<int>&& distances, std::vector<int>&& parents,
                 std::vector<std::vector<int>>&& children);
};

/* Returns a pair of distances and parents. Parents -1 mean unreachable,
 * aside from start.
 */
DijkstraOutput
dijkstra(const Graph<WeightedEdge>& graph, const int start);

#endif /* __DIJKSTRA__ */
