#include "graph.h"
#include "highway-dimension.h"

#include <cinttypes>
#include <cstdio>
#include <fstream>
#include <iostream>

// not enum class on purpose
enum EXIT_CODES {
  OK, INVALID_INPUT, COULD_NOT_OPEN_FILE
};

int
main(int argc, char* argv[])
{
  if (argc != 3) {
    std::cerr<<"Usage: "<<argv[0]<<" graph.in output.out"<<std::endl;
    return EXIT_CODES::INVALID_INPUT;
  }

  std::ifstream inputFile(argv[1]);
  if (!inputFile.good()) {
    std::cerr<<"Could not open input graph."<<std::endl;
    return EXIT_CODES::COULD_NOT_OPEN_FILE;
  }

  std::ofstream outputFile(argv[2]);
  if (!outputFile.good()) {
    std::cerr<<"Could not open output file."<<std::endl;
    return EXIT_CODES::COULD_NOT_OPEN_FILE;
  }

  Graph<WeightedEdge> G;
  #ifdef DEBUG
  fputs("Processing graph.....", stderr);
  #endif
  int u;
  int v;
  int64_t w;
  int64_t edge_count = 0;
  while(inputFile>>u>>v>>w){
    if(++edge_count%1000000==0){
      std::cerr<<"Read "<<edge_count<<" edges"<<std::endl;
    }
    G.addEdge(u, {v, w});
    G.addEdge(v, {u, w});
  }
  #ifdef DEBUG
  fputs("done\n", stderr);
  #endif

  outputFile<<G.vertex_count()<<" "<<G.edge_count()<<" "<<approximateHd(G)<<std::endl;

  return 0;
}
