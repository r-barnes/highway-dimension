#include "dijkstra.h"
#include "graph.h"

#include <climits>
#include <queue>
#ifdef DEBUG
#include <cstdio>
#endif

DijkstraOutput::DijkstraOutput(std::vector<int>&& distances,
                               std::vector<int>&& parents,
                               std::vector<std::vector<int>>&& children)
  : distances(std::move(distances)), parents(std::move(parents)),
      children(std::move(children))
{
}

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
  //              v-- priority queue is max
  return distance > o.distance || (distance == o.distance && vertex < o.vertex);
}

// TODO: visited mark
DijkstraOutput
dijkstra(const Graph<WeightedEdge>& graph, const int start)
{
  #ifdef DEBUG
  fprintf(stderr, "Started Dijkstra from %d.\n", start);
  #endif
  const int vertexCnt = graph.vertexCnt;
  std::vector<int> distances(vertexCnt, INT_MAX); 
  std::vector<int> parents(vertexCnt, voidParent);
  std::vector<std::vector<int>> children(vertexCnt);
  std::priority_queue<VertexDistance> Q;
  const int initialDistance = 0;
  distances[start] = initialDistance;
  parents[start] = voidParent;
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
        children[curV].push_back(e.to);
        Q.push({e.to, newDistance});
      }
    }
  }

  #ifdef DEBUG
  fprintf(stderr, "Finished Dijkstra from %d.\n", start);
  #endif
  return DijkstraOutput(std::move(distances), std::move(parents),
                        std::move(children));
}

std::set<std::set<int>>
collectShortestPaths(const DijkstraOutput& output, int start, int rFrom,
                     int rTo)
{
  const std::vector<int>& distances = output.distances;
  const std::vector<int>& parents = output.parents;
  const std::vector<std::vector<int>>& children = output.children;
  const int vertexCnt = distances.size();

  std::vector<bool> visited(vertexCnt, false);
  std::queue<int> Q;
  Q.push(start);
  std::set<std::set<int>> paths;

  while (!Q.empty()) {
    const int cur = Q.front();
    Q.pop();
    visited[cur] = true;

    for (const int neighbor: children[cur]) {
      if (visited[neighbor]) {
        continue;
      }
      const int neighborDistance = distances[neighbor];
      if (neighborDistance > rTo) {
        continue;
      }

      // FIXME: if parent is admissible, then just add me to his path
      if (rTo >= neighborDistance && neighborDistance > rFrom) {
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
