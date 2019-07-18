#ifndef __HITTING_SET__
#define __HITTING_SET__

#include "graph.h"

#include <vector>

/* Calculates an approximation of HITTING SET problem.
 * 
 * First `shooterCnt` vertices of G are shooters, the rest are targets.
 */
std::vector<int>
approximateHittingSet(const int shooterCnt, const Graph& G);

#endif /* __HITTING_SET__ */
