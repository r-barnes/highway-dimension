#include "graph.h"
#include "dijkstra.h"

#include <climits>
#include <queue>

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
std::pair<std::vector<int>, std::vector<int>>
dijkstra(const Graph& graph, const int start)
{
  int N = graph.vertexCnt;
  std::vector<int> distances(N, INT_MAX), parents(N, -1);
  std::priority_queue<VertexDistance> Q;
  int initialDistance = 0;
  distances[start] = initialDistance;
  Q.push({start, initialDistance});

  while (!Q.empty()) {
    const VertexDistance cur = Q.top();
    Q.pop();
    int curV = cur.vertex, curD = cur.distance;
    for (const Edge& e : graph.neighborList[curV]) {
      int newDistance = curD + e.weight;
      if (distances[e.to] > newDistance) {
        distances[e.to] = newDistance;
        parents[e.to] = curV;
        Q.push({e.to, newDistance});
      }
    }
  }

  return {distances, parents};
}
