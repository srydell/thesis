#ifndef MAIN_H
#define MAIN_H

#include "Graph.h"
#include <unordered_map>
#include <vector>

bool IsAccepted(double K, bool link_between, long double &random_num);
void UpdateCorrelationFunction(unsigned site0, unsigned site1, std::unordered_map<unsigned, unsigned> &correlation_func);
void UpdateLoopLengths(std::vector<double> &loop_lengths, std::unordered_map<unsigned, std::vector<unsigned>> &clusters, Graph &lattice);
double GetAverageLoopLength(std::vector<double> &loop_lengths);

#endif
