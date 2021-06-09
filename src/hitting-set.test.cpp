#include "graph.h"
#include "hitting-set.h"

#include <cassert>
#include <cstdio>
#include <random>
#include <unordered_set>
#include <utility>
#include <vector>

/* Pick a small enough, because runtime is square of this.
 */
const int instanceMaxSize = 10000;

struct HittingSetInstance
{
  const size_t reference;
  const int vertexCnt;
  const int shooterCnt;
  const std::vector<std::pair<int, int>> edges;
};

const std::vector<HittingSetInstance> instances = {
  {
    2, 9, 5, {
      {0, 5}, {1, 5}, {1, 6}, {1, 7}, {2, 5}, {3, 7}, {4, 7}, {4, 8}
    }
  },
  {
    3, 19, 14, {
      {0, 14}, {0, 17}, {1, 14}, {1, 18},
      {2, 15}, {2, 17}, {4, 15}, {4, 17},
      {3, 16}, {3, 18}, {5, 16}, {5, 18},
      {6, 16}, {6, 17}, {8, 16}, {8, 17}, {10, 16}, {10, 17}, {12, 16}, {12, 17},
      {7, 16}, {7, 18}, {9, 16}, {9, 18}, {11, 16}, {11, 18}, {13, 16}, {13, 18}
    }
  },
  {
    3, 10, 4, {
      {0, 4}, {0, 8}, {0, 9}, {1, 4}, {1, 6}, {1, 8}, {2, 4}, {2, 5}, {2, 8},
      {2, 9}, {3, 4}, {3, 5}, {3, 7}
    }
  }
};

void
randomInstance()
{
  std::random_device rd;
  std::mt19937 engine(rd());
  std::uniform_int_distribution<int> coinFlip(0, 1);
  std::uniform_int_distribution<int> instanceDistr(1, instanceMaxSize);
  const int instanceSize = instanceDistr(engine);
  std::uniform_int_distribution<int> shooterDistr(1, instanceSize);
  const int shooterCnt = shooterDistr(engine);
  #ifdef DEBUG
  const int targetCnt = instanceSize - shooterCnt;
  fprintf(stderr, "Shooter: %d, target: %d.\n", shooterCnt, targetCnt);
  #endif
  Graph<int> G(instanceSize);
  std::unordered_set<int> ref;
  for (int u = 0; u < shooterCnt; ++u) {
    for (int v = shooterCnt; v < instanceSize; ++v) {
      if (coinFlip(engine)) {
        G.addEdge(u, v);
        G.addEdge(v, u);
        ref.insert(v);
      }
    }
  }

  std::vector<int> hittingSet = approximateHittingSet(shooterCnt, G);
  std::unordered_set<int> solution;
  for (const int u: hittingSet) {
    for (const int v: G[u]) {
      solution.insert(v);
    }
  }

  #ifdef DEBUG
  if (ref != solution) {
    fputs("Offending test case:\n", stderr);
    for (int u = 0; u < shooterCnt; ++u) {
      for (const int v: G[u]) {
        fprintf(stderr, "%d %d\n", u, v);
      }
    }
    fputs("Found hitting set: ", stderr);
    for (const int u: hittingSet) {
      fprintf(stderr, "%d ", u);
    }
    fputs("\n", stderr);
  }
  #endif
  assert(ref == solution);
}

int
main(int argc, char* argv[])
{
  fputs("\n===== Hitting set test =====\n", stderr);
  for (const HittingSetInstance& instance: instances) {
    Graph<int> G(instance.vertexCnt);
    for (const std::pair<int, int>& e: instance.edges) {
      G.addEdge(e.first, e.second);
      G.addEdge(e.second, e.first);
    }
    const std::vector<int> solution =
      approximateHittingSet(instance.shooterCnt, G);
    assert(instance.reference == solution.size());
  }
  fputs("Fixed tests successful.\n", stderr);

  for (int i = 0; i < 1000; ++i) {
    randomInstance();
  }
  fputs("Random tests successful.\n", stderr);

  fputs("Tests successful.\n\n", stderr);
  return 0;
}
