#ifndef __DIJKSTRA__
#define __DIJKSTRA__

#include "graph.h"

#include <utility>
#include <vector>

/* Returns a pair of distances and parents. Parents -1 mean unreachable,
 * aside from start.
 */
std::pair<std::vector<int>, std::vector<int>>
dijkstra(const Graph<WeightedEdge>& graph, const int start);

#endif /* __DIJKSTRA__ */
