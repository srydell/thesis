#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <sstream>
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
	unsigned largest_index = 0;
	bool index_has_changed = 1;

	while (index_has_changed) {
		index_has_changed = 0;

		for (Site site : mGraph) {
			// Initialize neighbours
			std::vector<unsigned> neighbours;
			GetLinkedNeighbours(site.GetIndex(), neighbours);

			// Used to store site + neighbours if there are any neighbours
			std::vector<unsigned> local_cluster;

			// std::cout << "Handling site : " << site.GetIndex() << "\n";

			if (!neighbours.empty()) {
				// Found a cluster

				// local_cluster should be [current_site, neighbour0, neighbour1, ...]
				// Add the current site
				local_cluster.push_back(site.GetIndex());
				for (unsigned neighbour : neighbours) {

					// std::cout << "Found neighbour : " << neighbour << "\n";

					// Add the all the neighbours
					local_cluster.push_back(neighbour);
				}

				std::cout << "On site: " << site.GetIndex() << "\n";
				std::cout << "After initialization, local_cluster is: ";
				for (auto n : local_cluster) {
					std::cout << n << " ";
				}
				std::cout << "\n";

				// // Add the local cluster to clusters
				// clusters.insert({largest_index, local_cluster});

				// Populate indices with the indices of the sites in local_cluster.
				// These are found in clusters.
				// If not indexed before, indices has size = 0.
				std::vector<unsigned> indices;
				FindIndices(clusters, local_cluster, indices);

				// If no sites were indexed before
				if (indices.size() == 0) {
					index_has_changed = 1;
					largest_index++;

					// std::cout << "Largest index is now: " << largest_index << "\n";

					// Add the sites to the new index
					clusters.insert({largest_index, local_cluster});

					std::cout << "Inserted index: " << largest_index << " into clusters with local_cluster." << "\n";
					std::cout << "Clusters is now:" << "\n";
					PrintClusters(clusters);
					std::cout << "\n";

					// Go to next site
					continue;
				}
				// Here at least one of the sites are indexed

				// Get the smallest index that will be the index of the whole local_clusters
				unsigned smallest_index = *std::min_element(indices.begin(), indices.end());

				// std::cout << "Smallest index is : " << smallest_index << "\n";
				// std::cout << "local_cluster size is : " << local_cluster.size() << "\n";
				// std::cout << "indices size is : " << indices.size() << "\n";

				// If not all are indexed or not all have the smallest_index
				if ((local_cluster.size() != indices.size()) || !(AllHaveIndex(smallest_index, indices))) {

					std::cout << "Going to call move since:" << "\n";
					std::cout << "local_cluster.size() != indices.size() : " <<
						local_cluster.size() << " != " << indices.size() << "\n";
					std::cout << "Or" << "\n";
					std::cout << "All do not have smallest index. Smallest_index : " <<
						smallest_index << "\n";
					std::cout << "indices : ";
					for (unsigned i : indices) {
						std::cout << i << " ";
					}
					std::cout << "\n";
					std::cout << "Clusters before move" << "\n";
					PrintClusters(clusters);

					// Move all sites in local_cluster to smallest_index
					MoveToIndex(smallest_index, local_cluster, clusters);

					std::cout << "Clusters after move" << "\n";
					PrintClusters(clusters);

				}
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
	// Loop over all index : [site0, site1, ...] in clusters
	for (auto index_and_sites : clusters) {
		// Loop over all sites for that index in clusters
		for (unsigned site : index_and_sites.second) {

			if (IsInVector(site, local_cluster)) {
				// Found site in local_cluster, so add the corresponding index
				indices.push_back(index_and_sites.first);
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

/**
* @brief: Loop through indices_to_search and check if all have index
*
* @param: unsigned index
*       : std::vector<unsigned> &indices_to_search
*
* @return: bool
*/
bool Graph::AllHaveIndex(unsigned index, std::vector<unsigned> &indices_to_search) {
	for (unsigned i : indices_to_search) {
		if (i != index) {
			return 0;
		}
	}
	return 1;
}

/**
* @brief: Move all sites in local_cluster to smallest_index in clusters.
*         Remove all the occurances of the sites in other indices
*
* @param: unsigned smallest_index
*       : std::vector<unsigned> &local_cluster
*       : std::unordered_map<unsigned, std::vector<unsigned>> &clusters
*
* @return: void
*/
void Graph::MoveToIndex(unsigned smallest_index, std::vector<unsigned> &local_cluster, std::unordered_map<unsigned, std::vector<unsigned>> &clusters) {
	std::cout << "MoveToIndex called" << "\n";
	// Add the sites to smallest_index in clusters
	for (unsigned site_in_local_cluster : local_cluster) {
		// If site_in_local_cluster is not already in clusters for smallest_index
		if (!IsInVector(site_in_local_cluster, clusters[smallest_index])) {
			// Add the site
			clusters[smallest_index].push_back(site_in_local_cluster);
		}
	}

	// Loop through and record any sites from cluster indices that are not smallest_index
	std::unordered_map<unsigned, std::vector<unsigned>> indices_and_sites_to_remove;
	for (auto index_and_sites : clusters) {
		for (unsigned site : index_and_sites.second) {

			std::cout << "Handling site: " << site << "\n";

			// TODO: FIND THE BUG
			// if site in local_cluster AND it is not under the smallest_index
			if (IsInVector(site, local_cluster) && (smallest_index != index_and_sites.first)) {
				// Here we know that the site is under the wrong index

				std::cout << "Site " << site << " is in the wrong index. It has: " << index_and_sites.first
					<< ". And should have: " << smallest_index << "\n";

				// Check if the index of the site that should be removed
				// exists as a key in indices_and_sites_to_remove
				auto iterator = indices_and_sites_to_remove.find(index_and_sites.first);
				if (iterator != indices_and_sites_to_remove.end()) {
					// Create a vector of indices to remove
					std::vector<unsigned> sites_to_remove;
					sites_to_remove.push_back(site);

					// Add this vector together with the index as a key to indices_and_sites_to_remove
					indices_and_sites_to_remove.insert( {index_and_sites.first, sites_to_remove} );

					std::cout << "Added site " << site << " under a new index " << 
						index_and_sites.first << " to be removed" << "\n";

				} else {
					// Add site so it can be removed later
					indices_and_sites_to_remove[index_and_sites.first].push_back(site);

					std::cout << "Added site " << site << " under an old index " << 
						index_and_sites.first << " to be removed" << "\n";

				}

			} else {
				std::cout << "Site " << site << " is in the right index. It has: " << index_and_sites.first
					<< ". And should have: " << smallest_index << "\n";
			}
		}
	}

	// Now actually remove the sites
	for (auto index_and_sites : indices_and_sites_to_remove) {
		for (unsigned site : index_and_sites.second) {
			auto start = clusters[index_and_sites.first].begin();
			auto end = clusters[index_and_sites.first].end();
			// Search through and remove site from that cluster index
			clusters[index_and_sites.first].erase(std::remove(start, end, site));

			std::cout << "Removed site: " << site << " from cluster index: " << index_and_sites.first << "\n";
		}
		// Check if the index is now empy, if it is, remove it
		if (clusters[index_and_sites.first].size() == 0) {
			std::cout << "Removed index : " << index_and_sites.first << " from clusters totally." << "\n";
			clusters.erase(index_and_sites.first);
		}
	}
}

/**
* @brief: Pretty print an unordered_map<unsigned, vector<unsigned>>
*
* @param: std::unordered_map<unsigned, std::vector<unsigned>> &to_print
*
* @return: void
*/
void Graph::PrintClusters(std::unordered_map<unsigned, std::vector<unsigned>> &to_print) {
	std::stringstream ss;
	for (auto value_and_vector : to_print) {
		auto size = value_and_vector.second.size();

		ss << value_and_vector.first << " : { (";
		for(std::vector<int>::size_type i = 0; i != value_and_vector.second.size(); i++) {
			/* std::cout << someVector[i]; ... */
			ss << value_and_vector.second[i];
			if (i != (size - 1))
				ss << ", ";
		}
		ss << ") }\n";
	}
	std::cout << ss.str();
}
