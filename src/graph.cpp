#include "graph.h"

#include <cstring>

Graph::Graph(int n) : vertexCnt(n),
                      neighborList(vertexCnt)
{
}

std::vector<Edge>&
Graph::operator[](size_t index)
{
  return neighborList[index];
}

const std::vector<Edge>&
Graph::operator[](size_t index) const
{
  return neighborList[index];
}
