#include "Graph.h"

/**
* @brief: Loop through every cluster and see if there are two or more sites that are on different borders
*
* @param: std::unordered_map<unsigned, std::vector<unsigned>> const &clusters
*       : std::vector<unsigned> &percolating_indices
*
* @return: void
*/
void Graph::GetPercolatingIndices(std::unordered_map<unsigned, std::vector<unsigned>> const &clusters, std::vector<unsigned> &percolating_indices) {
	for (auto& index_and_cluster : clusters) {
		// Stores which borders this cluster has been on
		std::vector<bool> on_borders(2 * mDimension, 0);
		for (auto& site : index_and_cluster.second) {
			auto xyz = GetxyzConversion(site);
			percolating_indices.push_back(site);
		}
	}
}
