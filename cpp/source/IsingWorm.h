#ifndef ISINGWORM_H
#define ISINGWORM_H

#include "Graph.h"
#include <unordered_map>
#include <vector>

bool IsAccepted(double K, bool link_between, long double &random_num);
void UpdateCorrelationFunction(unsigned site0, unsigned site1, unsigned length, std::unordered_map<unsigned, unsigned> &correlation_func);
void UpdateLoopLengths(std::vector<unsigned> &loop_lengths, std::unordered_map<unsigned, std::vector<unsigned>> &clusters, Graph &lattice);
double GetAverageLoopLength(std::vector<unsigned> &loop_lengths, double const &K);

#endif
