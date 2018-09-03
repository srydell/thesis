#include "Graph.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

/**
* @brief: Loop through and index every connected site in graph. Add them to clusters as site : [neighbour0, neighbour1,...]
*
* @param : std::unordered_map<int, std::vector<int>> &clusters
*
* @return: void
*/
void Graph::IndexClusters(std::unordered_map<int, std::vector<int>> &clusters) {
	int largest_index = 0;
	bool index_has_changed = 1;

	// int num_loops = 0;

	while (index_has_changed) {
		index_has_changed = 0;

		// std::cout << "On loop: " << ++num_loops << "\n";

		for (Site site : mGraph) {
			// Initialize neighbours
			std::vector<int> neighbours;
			GetLinkedNeighbours(site.GetIndex(), neighbours);

			// Used to store site + neighbours if there are any neighbours
			std::vector<int> local_cluster;

			// std::cout << "Handling site : " << site.GetIndex() << "\n";

			if (!neighbours.empty()) {
				// Found a cluster

				// local_cluster should be [current_site, neighbour0, neighbour1, ...]
				// Add the current site
				local_cluster.push_back(site.GetIndex());
				for (int neighbour : neighbours) {

					// std::cout << "Found neighbour : " << neighbour << "\n";

					// Add the all the neighbours
					local_cluster.push_back(neighbour);
				}

				// std::cout << "On site: " << site.GetIndex() << "\n";
				// std::cout << "After initialization, local_cluster is: ";
				// for (auto n : local_cluster) {
				// 	std::cout << n << " ";
				// }
				// std::cout << "\n";

				// Populate indices with the indices of the sites in local_cluster.
				// These are found in clusters.
				// If not indexed before, indices has size = 0.
				std::vector<int> indices;
				FindIndices(clusters, local_cluster, indices);

				// If no sites were indexed before
				if (indices.size() == 0) {
					// std::cout << "Not indexed before" << "\n";
					index_has_changed = 1;
					largest_index++;

					// std::cout << "Largest index is now: " << largest_index << "\n";

					// Add the sites to the new index
					// clusters.insert({largest_index, local_cluster});
					// TODO: See if this way is better by writing a test where a worm overwrites an earlier configuration
					clusters[largest_index] = local_cluster;

					// std::cout << "Inserted index: " << largest_index << " into clusters with local_cluster." << "\n";
					// std::cout << "Clusters is now:" << "\n";
					// PrintClusters(clusters);
					// std::cout << "\n";

					// Go to next site
					continue;
				}
				// Here at least one of the sites are indexed

				// Get the smallest index that will be the index of the whole local_clusters
				int smallest_index = *std::min_element(indices.begin(), indices.end());

				// std::cout << "Smallest index is : " << smallest_index << "\n";
				// std::cout << "local_cluster size is : " << local_cluster.size() << "\n";
				// std::cout << "indices size is : " << indices.size() << "\n";

				// If not all are indexed or not all have the smallest_index
				if ((local_cluster.size() != indices.size()) || !(AllHaveIndex(smallest_index, indices))) {

					// std::cout << "Going to call move since:" << "\n";
					// std::cout << "local_cluster.size() != indices.size() : " <<
					// 	local_cluster.size() << " != " << indices.size() << "\n";
					// std::cout << "Or" << "\n";
					// std::cout << "All do not have smallest index. Smallest_index : " <<
					// 	smallest_index << "\n";
					// std::cout << "indices : ";
					// for (int i : indices) {
					// 	std::cout << i << " ";
					// }
					// std::cout << "\n";
					// std::cout << "Clusters before move" << "\n";
					// PrintClusters(clusters);

					index_has_changed = 1;
					// Move all sites in local_cluster to smallest_index
					MoveToIndex(smallest_index, local_cluster, clusters);

					// std::cout << "Clusters after move" << "\n";
					// PrintClusters(clusters);

				}
			} else {
				// This site has no neighbours
				// If it exists in clusters, remove it
				// TODO: Make this work
				for (auto& index_and_sites : clusters) {
					if (IsInVector(site.GetIndex(), clusters[index_and_sites.first])) {
						auto& v = clusters[index_and_sites.first];
						const auto& it = std::find(v.begin(), v.end(), site.GetIndex());
						clusters[index_and_sites.first].erase(it);
					}
				}
			}
		}
	}

	// RemoveDeprecated(clusters);

}

/**
* @brief: Loop over the sites in local_cluster and populate indices if the site can be found under some index in clusters
*
* @param: std::unordered_map<int, std::vector<int>> &clusters
*       : std::vector<int> &local_cluster
*       : std::vector<int> &indices
*
* @return: void
*/
void Graph::FindIndices(std::unordered_map<int, std::vector<int>> &clusters, std::vector<int> &local_cluster, std::vector<int> &indices) {
	// Loop over all index : [site0, site1, ...] in clusters
	for (auto index_and_sites : clusters) {
		// Loop over all sites for that index in clusters
		for (int site : index_and_sites.second) {

			if (IsInVector(site, local_cluster)) {
				// Found site in local_cluster, so add the corresponding index
				indices.push_back(index_and_sites.first);
			}
		}
	}
}

/**
* @brief: Returns 1 if item can be found in vector_to_search
*
* @param: std::vector<int> vector_to_search
*       : int item
*
* @return: bool
*/
bool Graph::IsInVector(int item, std::vector<int> &vector_to_search) {
	return std::find(vector_to_search.begin(), vector_to_search.end(), item) != vector_to_search.end();
}

/**
* @brief: Loop through indices_to_search and check if all have index
*
* @param: int index
*       : std::vector<int> &indices_to_search
*
* @return: bool
*/
bool Graph::AllHaveIndex(int index, std::vector<int> &indices_to_search) {
	for (int i : indices_to_search) {
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
* @param: int smallest_index
*       : std::vector<int> &local_cluster
*       : std::unordered_map<int, std::vector<int>> &clusters
*
* @return: void
*/
void Graph::MoveToIndex(int smallest_index, std::vector<int> &local_cluster, std::unordered_map<int, std::vector<int>> &clusters) {
	// std::cout << "MoveToIndex called" << "\n";
	// Add the sites to smallest_index in clusters
	for (int site_in_local_cluster : local_cluster) {
		// If site_in_local_cluster is not already in clusters for smallest_index
		if (!IsInVector(site_in_local_cluster, clusters[smallest_index])) {
			// Add the site
			clusters[smallest_index].push_back(site_in_local_cluster);
		}
	}

	// Loop through and record any sites from cluster indices that are not smallest_index
	std::unordered_map<int, std::vector<int>> indices_and_sites_to_remove;
	for (auto index_and_sites : clusters) {

		// If it is not the smallest index we want to look for the sites in local_cluster so we can remove them
		if (index_and_sites.first != smallest_index) {
			for (int local_site : local_cluster) {
				auto begin = index_and_sites.second.begin();
				auto end = index_and_sites.second.end();
				// Check if local_site is in this index
				// If it is, it should be removed
				if (std::find(begin, end, local_site) != end) {
					// std::cout << "Found site: " << local_site << " that should be removed" << "\n";
					indices_and_sites_to_remove[index_and_sites.first].push_back(local_site);
					// std::cout << "So now indices_and_sites_to_remove is as:" << "\n";
					// PrintClusters(indices_and_sites_to_remove);
				}
			}
		}

	}

	// Now actually remove the sites
	for (auto index_and_sites : indices_and_sites_to_remove) {
		for (int site : index_and_sites.second) {
			// std::cout << "Want to delete site: " << site << "\n";
			auto start = clusters[index_and_sites.first].begin();
			auto end = clusters[index_and_sites.first].end();
			// Search through and remove site from that cluster index
			clusters[index_and_sites.first].erase(std::remove(start, end, site));

			// std::cout << "Removed site: " << site << " from cluster index: " << index_and_sites.first << "\n";
		}
		// Check if the index is now empy, if it is, remove it
		if (clusters[index_and_sites.first].size() == 0) {
			// std::cout << "Removed index : " << index_and_sites.first << " from clusters totally." << "\n";
			clusters.erase(index_and_sites.first);
		}
	}
}

/**
* @brief: Pretty print an unordered_map<int, vector<int>>
*
* @param: std::unordered_map<int, std::vector<int>> &to_print
*
* @return: void
*/
void Graph::PrintClusters(std::unordered_map<int, std::vector<int>> &to_print) {
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
