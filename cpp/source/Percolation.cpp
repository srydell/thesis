#include "Graph.h"
#include <cmath>

/**
* @brief: Loop through every cluster and add any percolating cluster indices to percolating_indices
*
* @param: std::unordered_map<int, std::vector<int>> const &clusters
*       : std::vector<int> &percolating_indices
*
* @return: void
*/
void Graph::GetPercolatingIndices(std::unordered_map<int, std::vector<int>> const &clusters, std::vector<int> &percolating_indices) {
	for (auto& index_and_cluster : clusters) {

		// std::cout << "\n\nOn index: " << index_and_cluster.first << "\n";

		// Stores which borders this cluster has been on
		for (auto& site : index_and_cluster.second) {

			// std::cout << "On site: " << site << "\n";

			std::vector<int> neighbours;
			GetLinkedNeighbours(site, neighbours);

			// If a neighbour to site is in perc_neighbours,
			// then the cluster is percolating
			std::vector<int> perc_neighbours;
			perc_neighbours.reserve(mDimension);
			// perc_0 = site + (L-1) * L^0
			// perc_1 = site + (L-1) * L^1
			// ...
			// perc_(d-1) = site + (L-1) * L^(d-1)
			// NOTE: This only adds one direction per dimension (+x, +y, +z, ...)
			//       But since site -> neighbour => neighbour -> site
			//       It is ensured to find all percolating clusters

			// std::cout << "If percolating, it should have one of: " << "\n";

			for (int i = 0; i < mDimension; ++i) {
				perc_neighbours.push_back(site + (mLength - 1) * std::pow(mLength, i));

				// std::cout << site + (mLength - 1) * std::pow(mLength, i) << ", ";

			}

			// std::cout << "as a neighbour" << "\n";

			bool perc_cluster = 0;
			for (auto& neighbour : neighbours) {
				if (IsInVector(neighbour, perc_neighbours)) {

					// std::cout << "Found neighbour: " << neighbour << " in perc_neighbours." << "\n";

					perc_cluster = 1;
					break;
				}
			}

			if (perc_cluster) {

				// std::cout << "Therefore adding index: " << index_and_cluster.first << " to percolating_indices." << "\n";

				percolating_indices.push_back(index_and_cluster.first);
				break;
			}
		}
	}
}
