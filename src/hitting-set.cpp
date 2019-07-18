#include "hitting-set.h"

#include <unordered_set>
#include <algorithm>

struct Shooter {
  int id;
  int cnt;

  bool operator<(const Shooter& s) const;
};

bool
Shooter::operator<(const Shooter& s) const
{
  return cnt < s.cnt;
}

std::vector<int>
approximateHittingSet(const int shooterCnt, const Graph& G)
{
  std::vector<Shooter> H(shooterCnt);
  std::vector<int> realDegrees(shooterCnt);
  for (int i = 0; i < shooterCnt; ++i) {
    H[i].id = i;
    H[i].cnt = realDegrees[i] = G[i].size();
  }
 
  std::vector<int> hittingSet;
  std::make_heap(H.begin(), H.end());
  
  while (!H.empty()) {
    std::pop_heap(H.begin(), H.end());
    const Shooter mx = H.back();
    H.pop_back();
    if (!mx.cnt) {
      break;
    }

    if (realDegrees[mx.id] < mx.cnt) {
      H.push_back({mx.id, realDegrees[mx.id]});
      std::push_heap(H.begin(), H.end());
      continue;
    }

    hittingSet.push_back(mx.id);
    for (const Edge& e : G[mx.id]) {
      for (const Edge& e2: G[e.to]) {
        --realDegrees[e2.to];
      }
    }
  }

  return hittingSet;
}
