#include "graph.h"

#include <cstdio>
#include <unordered_set>

int
main(int argc, char* argv[])
{
  int N, M;
  scanf("%d%d", &N, &M);

  Graph<WeightedEdge> G(N);

  for (int i = 0; i < M; ++i) {
    int u, v, w;
    scanf("%d%d%d", &u, &v, &w);
    G.addEdge(u, {v, w});
    G.addEdge(v, {u, w});
  }
}
