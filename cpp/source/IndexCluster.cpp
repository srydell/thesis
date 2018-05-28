#include "Graph.h"
#include <iostream>
#include <unordered_map>
#include <vector>
#include <algorithm>

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

				}

				// Add the local cluster to clusters
				clusters.insert({largestIndex, local_cluster});

				std::vector<unsigned> indices;
				FindIndices(clusters, local_cluster, indices);
				largestIndex++;

			}
		}
	}
}

/**
* @brief: Loop over the sites in local_cluster and populate indices if the site can be found under some index in clusters
*
* @param: std::unordered_map<unsigned, std::vector<unsigned>> &clusters
*       : std::vector<unsigned> &local_cluster
*       : std::vector<unsigned> &indices
*
* @return: void
*/
void Graph::FindIndices(std::unordered_map<unsigned, std::vector<unsigned>> &clusters, std::vector<unsigned> &local_cluster, std::vector<unsigned> &indices) {

	std::cout << "Cluster size is : " << clusters.size() << "\n";
	std::cout << "Local cluster size is : " << local_cluster.size() << "\n";
	std::cout << "Indices size is : " << indices.size() << "\n";

	// Loop over all index : [site0, site1, ...] in clusters
	for (auto index_and_sites : clusters) {
		// Loop over all sites for that index in clusters
		for (unsigned site : index_and_sites.second) {

			std::cout << "Site in index_and_sites is : " << site << "\n";
			if (IsInVector(site, local_cluster)) {
				std::cout << "Site is in local_cluster! " << site << "\n";
				indices.push_back(site);
			}
		}
	}
	std::cout << "\n";
}

/**
* @brief: Returns 1 if item can be found in vector_to_search
*
* @param: std::vector<unsigned> vector_to_search
*       : unsigned item
*
* @return: bool
*/
bool Graph::IsInVector(unsigned item, std::vector<unsigned> &vector_to_search) {
	return std::find(vector_to_search.begin(), vector_to_search.end(), item) != vector_to_search.end();
}
