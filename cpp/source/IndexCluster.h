#ifndef INDEXCLUSTER_H
#define INDEXCLUSTER_H

#include "Graph.h"
#include <iostream>
#include <unordered_map>
#include <vector>

void IndexClusters(Graph graph, const std::unordered_map<unsigned, std::vector<unsigned>> &clusters);

#endif
