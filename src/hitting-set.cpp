#include "hitting-set.h"

#include <algorithm>

struct Shooter
{
  int vertex;
  int degree;

  bool operator<(const Shooter& s) const;
};

bool
Shooter::operator<(const Shooter& s) const
{
  return degree < s.degree || (degree == s.degree && vertex < s.vertex);
}

std::vector<int>
approximateHittingSet(const int shooterCnt, const Graph<int>& G)
{
  const int targetCnt = G.vertexCnt - shooterCnt;
  std::vector<Shooter> pq(shooterCnt);
  std::vector<int> realDegrees(shooterCnt);
  std::vector<bool> covered(targetCnt, false);
  for (int i = 0; i < shooterCnt; ++i) {
    pq[i].vertex = i;
    pq[i].degree = realDegrees[i] = G[i].size();
  }
 
  std::vector<int> hittingSet;
  std::make_heap(pq.begin(), pq.end());
  
  while (!pq.empty()) {
    std::pop_heap(pq.begin(), pq.end());
    const int degree = pq.back().degree;
    const int vertex = pq.back().vertex;
    pq.pop_back();

    if (!degree) {
      break;
    }

    if (realDegrees[vertex] < degree) {
      pq.push_back({vertex, realDegrees[vertex]});
      std::push_heap(pq.begin(), pq.end());
      continue;
    }

    hittingSet.push_back(vertex);
    for (const int u: G[vertex]) {
      const int targetIndex = u - shooterCnt;
      if (covered[targetIndex]) {
        continue;
      }
      for (const int v: G[u]) {
        --realDegrees[v];
      }
      covered[targetIndex] = true;
    }
  }

  return hittingSet;
}
