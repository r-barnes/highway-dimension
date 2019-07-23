#include "graph.h"
#include "dijkstra.h"

#include <climits>
#include <queue>

const int voidParent = -1;

struct VertexDistance
{
  int vertex;
  int distance;

  bool operator<(const VertexDistance& o) const;
};

bool
VertexDistance::operator<(const VertexDistance& o) const
{
  return distance > o.distance; // priority queue is max
}

// TODO: visited mark
DijkstraOutput
dijkstra(const Graph<WeightedEdge>& graph, const int start)
{
  const int vertexCnt = graph.vertexCnt;
  std::vector<int> distances(vertexCnt, INT_MAX); 
  std::vector<int> parents(vertexCnt, voidParent);
  std::vector<std::vector<int>> next(vertexCnt);
  std::priority_queue<VertexDistance> Q;
  const int initialDistance = 0;
  distances[start] = initialDistance;
  Q.push({start, initialDistance});

  while (!Q.empty()) {
    const VertexDistance cur = Q.top();
    Q.pop();
    const int curV = cur.vertex, curD = cur.distance;
    for (const WeightedEdge& e : graph[curV]) {
      int newDistance = curD + e.weight;
      if (distances[e.to] > newDistance) {
        distances[e.to] = newDistance;
        parents[e.to] = curV;
        next[curV].push_back(e.to);
        Q.push({e.to, newDistance});
      }
    }
  }

  return {distances, parents};
}
