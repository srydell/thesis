#ifndef INDEXCLUSTER_H
#define INDEXCLUSTER_H

#include <unordered_map>
#include <vector>

void IndexClusters(const std::unordered_map<unsigned, std::vector<unsigned>> &clusters);
void FindIndices(std::unordered_map<unsigned, std::vector<unsigned>> &clusters, std::vector<unsigned> &local_cluster, std::vector<unsigned> &indices);
bool IsInVector(unsigned item, std::vector<unsigned> &vector_to_search);
bool AllHaveIndex(unsigned index, std::vector<unsigned> &indices_to_search);
void MoveToIndex(unsigned smallest_index, std::vector<unsigned> &local_cluster, std::unordered_map<unsigned, std::vector<unsigned>> &clusters);
void PrintClusters(std::unordered_map<unsigned, std::vector<unsigned>> &to_print);

#endif
