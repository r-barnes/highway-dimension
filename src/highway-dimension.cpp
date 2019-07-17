#include "graph.h"

#include <cstdio>

int
main(int argc, char* argv[])
{
  int N, M;
  scanf("%d%d", &N, &M);

  Graph G(N);

  for (int i = 0; i < M; ++i) {
    int u, v, w;
    scanf("%d%d%d", &u, &v, &w);
    G.neighborList[u].push_back({v, w});
    G.neighborList[v].push_back({u, w});
  }
}
