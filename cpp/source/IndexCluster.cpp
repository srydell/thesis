#include "Graph.h"
#include <unordered_map>
#include <vector>
#include <iostream>

/**
* @brief: Loop through and index every connected site in graph. Add them to clusters as site : [neighbour0, neighbour1, ...]
*
* @param : std::unordered_map<unsigned, std::vector<unsigned>> &clusters
*
* @return: void
*/
void Graph::IndexClusters(std::unordered_map<unsigned, std::vector<unsigned>> &clusters) {
	unsigned largestIndex = 0;
	bool indexHasChanged = 1;
	std::cout << clusters.size() << "\n";

	while (indexHasChanged) {
		indexHasChanged = 0;

		largestIndex += 1;

		for (Site site : mGraph) {
			std::vector<unsigned> neighbours = GetLinkedNeighbours(site.GetIndex());

			std::vector<unsigned> sitesInCluster;

			for (unsigned neighbour : neighbours) {
				std::cout << "hi" << "\n";

				std::cout << neighbour << "\n";

				sitesInCluster.push_back(neighbour);
				clusters.insert({site.GetIndex(), sitesInCluster});
			}
		}
	}
}
