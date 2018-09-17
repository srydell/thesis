#ifndef ISINGWORM_H
#define ISINGWORM_H

#include "Graph.h"
#include "xyHelper.h"
#include <unordered_map>
#include <vector>

bool IsAccepted(double K, int link_between, long double &random_num);
void UpdateLoopLengths(std::unordered_map<int, int> &loop_lengths, std::unordered_map<int, std::vector<int>> &clusters, Graph &lattice);
WNandNS XySimulation(Graph & lattice, double K);
void WarmUp(int warm_up_runs, Graph& lattice, double K);
void PopulateBoxDim(Graph& lattice, std::unordered_map<int, std::vector<int>>& clusters, std::unordered_map<int, std::vector<int>>& loop_lengths);

#endif
