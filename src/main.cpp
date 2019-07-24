#include "graph.h"
#include "highway-dimension.h"

#include <cstdio>

int
main(int argc, char* argv[])
{
  if (argc < 2) {
    fprintf(stderr, "Usage: %s graph.in\n", argv[0]);
    return 1;
  }
  // FIXME: read input properly
  freopen(argv[1], "r", stdin);
  int N, M;
  scanf("%d%d", &N, &M);

  Graph<WeightedEdge> G(N);

  #ifdef DEBUG
  fputs("Processing graph.....", stderr);
  #endif
  for (int i = 0; i < M; ++i) {
    int u, v;
    int64_t w;
    scanf("%d%d%" SCNd64, &u, &v, &w);
    G.addEdge(u, {v, w});
    G.addEdge(v, {u, w});
  }
  #ifdef DEBUG
  fputs("done\n", stderr);
  #endif

  printf("%d\n", approximateHd(G));
}
