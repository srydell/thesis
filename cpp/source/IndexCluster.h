#ifndef INDEXCLUSTER_H
#define INDEXCLUSTER_H

#include "Graph.h"
#include <iostream>
#include <unordered_map>
#include <vector>

void IndexClusters(const std::unordered_map<unsigned, std::vector<unsigned>> &clusters);
void FindIndices(std::unordered_map<unsigned, std::vector<unsigned>> &clusters, std::vector<unsigned> &local_cluster, std::vector<unsigned> &indices);

#endif
