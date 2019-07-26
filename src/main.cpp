#include "graph.h"
#include "highway-dimension.h"

#include <cstdio>
#include <cinttypes>

// not enum class on purpose
enum EXIT_CODES {
  OK, INVALID_INPUT, COULD_NOT_OPEN_FILE
};

int
main(int argc, char* argv[])
{
  if (argc < 3) {
    fprintf(stderr, "Usage: %s graph.in output.out\n", argv[0]);
    return EXIT_CODES::INVALID_INPUT;
  }
  FILE* inputFile = fopen(argv[1], "r");
  if (!inputFile) {
    fputs("Could not open input graph.\n", stderr);
    return EXIT_CODES::COULD_NOT_OPEN_FILE;
  }
  FILE* outputFile = fopen(argv[2], "w+");
  if (!outputFile) {
    fputs("Could not open output file.\n", stderr);
    return EXIT_CODES::COULD_NOT_OPEN_FILE;
  }

  int N, M;
  if (2 != fscanf(inputFile, "%d%d", &N, &M)) {
    fputs("Incorrect graph input.\n", stderr);
  }

  Graph<WeightedEdge> G(N);
  #ifdef DEBUG
  fputs("Processing graph.....", stderr);
  #endif
  for (int i = 0; i < M; ++i) {
    int u, v;
    int64_t w;
    if (3 != fscanf(inputFile, "%d%d%" SCNd64, &u, &v, &w)) {
      fputs("Incorrect edge input.\n", stderr);
    }
    G.addEdge(u, {v, w});
    G.addEdge(v, {u, w});
  }
  #ifdef DEBUG
  fputs("done\n", stderr);
  #endif

  fprintf(outputFile, "%d %d %d\n", N, M, approximateHd(G));
  fclose(inputFile);
  fclose(outputFile);
}
