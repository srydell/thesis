#include "Graph.h"
#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_map>

/**
* @brief: Index clusters according to a modified Hoshen Kopelman algorithm
*
* @param: std::unordered_map<unsigned, std::vector<unsigned>> &clusters
*
* @return: void
*/
void Graph::HKIndex(std::unordered_map<unsigned, std::vector<unsigned>> &clusters) {
	unsigned largest_label = 0;

	// Used to avoid indexing a cluster twice
	auto visited_clusters = std::make_unique<std::vector<bool>>(std::pow(mLength, mDimension), 0);
	
	for (Site &site : mGraph) {
		// Initialize neighbours
		std::vector<unsigned> neighbours;
		GetLinkedNeighbours(site.GetIndex(), neighbours);

		// Used to store site + neighbours if there are any neighbours
		std::vector<unsigned> local_cluster;

		std::cout << "Handling site : " << site.GetIndex() << "\n";

		if (!neighbours.empty()) {
			if (!(*visited_clusters)[site.GetIndex()]) {
				// Found a cluster that has not been identified
				// Conservative estimate on the size of the cluster
				local_cluster.reserve(neighbours.size() + 1);

				// local_cluster should be [current_site, neighbour0, neighbour1, ...]
				// Add the current site
				local_cluster.push_back(site.GetIndex());
				largest_label++;
				(*visited_clusters)[site.GetIndex()] = 1;

				// Go in all directions
				for (unsigned &neighbour : neighbours) {

					std::cout << "Found neighbour : " << neighbour << "\n";

					// Add whole cluster to local_cluster one direction at a time
					Union(neighbour, largest_label, local_cluster, visited_clusters);
				}

				std::cout << "On label: " << largest_label << "\n";
				std::cout << "Adding local_cluster:" << "\n";
				for (auto& value : local_cluster) {
					std::cout << value << ", ";
				}
				std::cout << "\n\n";

				// This cluster is fully found
				clusters[largest_label] = local_cluster;
			}
		}
	}
	PrintClusters(clusters);
}

/**
* @brief: Add neighbour to local_cluster if it is not labeled in visited_clusters, then call itself on each of neighbours neighbours
*
* @param: unsigned neighbour
*       : std::vector<unsigned> &local_cluster
*       : std::unique_ptr<std::vector<bool>>& visited_clusters
*
* @return: void
*/
void Graph::Union(unsigned neighbour, unsigned largest_label, std::vector<unsigned> &local_cluster, std::unique_ptr<std::vector<bool>> &visited_clusters) {

	std::cout << "\nCall to Union with parameters:" << "\n";
	std::cout << "neighbour: " << neighbour << "\n";
	std::cout << "largest_label: " << largest_label << "\n";

	// If not previously visited
	if (!(*visited_clusters)[neighbour]) {

		std::cout << "Neighbour not visited before. Adding to local_cluster." << "\n";

		// Mark as visited
		(*visited_clusters)[neighbour] = 1;
		local_cluster.push_back(neighbour);

		std::vector<unsigned> neighbours_to_neighbour;
		GetLinkedNeighbours(neighbour, neighbours_to_neighbour);

		std::cout << "Found neighbours to neighbour: " << neighbour << "\n";
		for (auto& value : neighbours_to_neighbour) {
			std::cout << value << ", ";
		}
		std::cout << "\n\n";

		for (auto& nn : neighbours_to_neighbour) {
			// Add whole cluster to local_cluster one direction at a time
			Union(nn, largest_label, local_cluster, visited_clusters);
		}
	}
}
