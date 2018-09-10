#ifndef ISINGWORM_H
#define ISINGWORM_H

#include "Graph.h"
#include <unordered_map>
#include <vector>

bool IsAccepted(double K, bool link_between, long double &random_num);
void UpdateCorrelationFunction(int site0, int site1, int length, std::unordered_map<int, int> &correlation_func);
void UpdateLoopLengths(std::unordered_map<int, int> &loop_lengths, std::unordered_map<int, std::vector<int>> &clusters, Graph &lattice);
double GetAverageLoopLength(std::vector<int> &loop_lengths, double const &K);
long double IsingSimulation(Graph & lattice, double K);
void WarmUp(int warm_up_runs, Graph& lattice, double K);

#endif
