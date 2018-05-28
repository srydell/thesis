#include "Graph.h"
#include <iostream>
#include <unordered_map>
#include <vector>

/**
* @brief: Loop through and index every connected site in graph. Add them to clusters as site : [neighbour0, neighbour1,...]
*
* @param : std::unordered_map<unsigned, std::vector<unsigned>> &clusters
*
* @return: void
*/
void Graph::IndexClusters(std::unordered_map<unsigned, std::vector<unsigned>> &clusters) {
	unsigned largestIndex = 0;
	bool indexHasChanged = 1;

	while (indexHasChanged) {
		indexHasChanged = 0;

		largestIndex += 1;

		for (Site site : mGraph) {
			// Initialize neighbours
			std::vector<unsigned> neighbours;
			GetLinkedNeighbours(site.GetIndex(), neighbours);

			// Used to store site + neighbours if there are any neighbours
			std::vector<unsigned> local_cluster;

			std::cout << "Handling site : " << site.GetIndex() << "\n";

			if (!neighbours.empty()) {
				// Found a cluster

				// Add the current site
				local_cluster.push_back(site.GetIndex());

				for (unsigned neighbour : neighbours) {

					std::cout << "Found neighbour : " << neighbour << "\n";

					// Add the all the neighbours to site
					local_cluster.push_back(neighbour);

					// Add the local cluster to clusters
					clusters.insert({largestIndex, local_cluster});
					largestIndex++;
				}
			}
		}
	}
}
