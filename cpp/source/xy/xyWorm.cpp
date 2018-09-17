#include "Graph.h"
#include "utils.h"
#include "xyHelper.h"
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_map>

/**
* @brief: brief
*
* @param: std::vector<int> 
*
* @return: void
*/
void PopulateBoxDim(Graph& lattice, std::unordered_map<int, std::vector<int>>& clusters, std::unordered_map<int, std::vector<int>>& loop_lengths) {
	// Find the sites corresponding to the largest worm
	std::vector<int> largest_worm = clusters[GetMaximumMapIndex(loop_lengths)];

	// Split up the graph into boxes of decreasing sizes
	std::unordered_map<int, std::vector<int>> blocks;
	lattice.DivideGraph(blocks);
	
	std::unordered_map<int, int> side_length_and_num_occupied;
	lattice.GetBoxDimension(blocks, side_length_and_num_occupied, largest_worm);
}

/**
* @brief: Check if the link between current_site and next_site in gitter is accepted
*
* @param: double K
*       : int current_site
*       : int next_site
*       : int link_between
*       : int difference
*       : long double random_num
*
* @return: bool
*/
bool IsAccepted(double K, int link_between, int difference, long double &random_num) {
	// std::cout << "Call to IsAccepted(K = " << K << ", link_between = " << link_between << ", difference = " << difference << ", random_num = " << random_num << "\n";

    // Probability of being accepted
	int new_energy = std::pow(link_between + difference, 2);
	int old_energy = std::pow(link_between, 2);
	double e = 2.7182818;
    auto p = std::pow(e, K * (new_energy - old_energy));

	// std::cout << "p before test: " << p << "\n";

	p = (p < 1) ? p : 1;

	// std::cout << "Probability of acceptance: p = " << p << "\n";
	// std::string s;
	// std::cin >> s;

	if (random_num < p) {
		// std::cout << "Accepted!" << "\n";
		return 1;
	} else {
		// std::cout << "Rejected!" << "\n";
		return 0;
	}
}

/**
* @brief: For each cluster in clusters, find the length of that loop
*
* @param: std::unordered_map<int, int> &loop_lengths
*       : std::unordered_map<int, std::vector<int> &clusters
*       : Graph &lattice
*
* @return: void
*/
void UpdateLoopLengths(std::unordered_map<int, int> &loop_lengths, std::unordered_map<int, std::vector<int>> &clusters, Graph &lattice) {
	//TODO: Test this function

	// std::cout << "Call to UpdateLoopLengths" << "\n";

	// Will be used to measure the length of all links from site
	std::vector<int> linked_neighbours;
	for (auto index_and_sites : clusters) {
		// Start each cluster with zero length
		int current_length = 0;

		// std::cout << "On index: " << index_and_sites.first << "\n";

		for (int site : index_and_sites.second) {
			// Find the number of links going through site
			// NOTE: This will be double counting since if
			//       site0 is a linked neighbour to site1
			//       site1 is a linked neighbour to site0.
			lattice.GetLinkedNeighbours(site, linked_neighbours);
			current_length += linked_neighbours.size();

			// std::cout << "Added site: " << site << "\n";
			// std::cout << "That has the neighbours: " << "\n";
			// for (auto n : linked_neighbours) {
			// 	std::cout << n << " ";
			// }
			// std::cout << "\nCurrent length is now: " << current_length << "\n";

			// Reinitialize the vector to be used on the next site
			linked_neighbours.clear();
		}

		// std::cout << "This cluster size is: " << current_length / 2 << "\n";

		// Divide by 2 to avoid double counting
		loop_lengths[index_and_sites.first] = current_length / 2;
	}
}

/**
* @brief: Run the simulation until at least one loop is formed and return the number of steps taken
*
* @param: Graph &lattice
*       : double K
*
* @return: WNandNS
*/
WNandNS XySimulation(Graph & lattice, double K) {

	// std::cout << "\n\nCall to XySimulation" << "\n";

	// TODO: Fix this to be XY
	// Get the first site for this simulation
	int first_site = lattice.GetRandomSite();

	// std::cout << "Got the first site: " << first_site << "\n";

	// Get some random neighbour to form the first link
	int current_site = std::abs(lattice.GetRandomNeighbour(first_site));

	// std::cout << "Got the neighbour: " << current_site << "\n";

	// Store the total links for this loop (winding number)
	int sign = lattice.GetSign(first_site, current_site);
	int winding_number = sign;

	// Form the first link
	lattice.SwitchLinkBetween(first_site, current_site);

	// Store the total number of accepted steps
	long double num_steps = 1.0;

	bool loop_formed = 0;
	while (!loop_formed) {
		int next_site = std::abs(lattice.GetRandomNeighbour(current_site));

		// std::cout << "(current_site, next_site) = (" << current_site << ", " << next_site << ")" << "\n";

		int sign = lattice.GetSign(current_site, next_site);
		auto rand_num = lattice.GetRandomNum();
		if (IsAccepted(K, lattice.GetLink(current_site, next_site), sign, rand_num)) {
			num_steps++;
			winding_number += sign;

			// std::cout << "Got accepted!" << "\n";
			// std::cout << "Switch link between sites: " << current_site << " and " << next_site << "\n";
			// std::cout << "Call lattice.SwitchLinkBetween(" << current_site << ", " << sign << " * " <<  next_site << ");" << "\n";

			// Flip the weight between currentSite and nextSite
			lattice.SwitchLinkBetween(current_site, next_site);
			current_site = next_site;

			// lattice.PrintGraph();

			// If we have found a loop
			if (next_site == first_site) {
				// std::cout << "Found a loop!" << "\n==========================\n\n";
				loop_formed = 1;
			}
		}
	}
	return {winding_number, num_steps};
}

/**
* @brief: Run warm_up_runs number of updates and throw them away
*
* @param: int warm_up_runs
*       : Graph& lattice
*
* @return: void
*/
void WarmUp(int warm_up_runs, Graph& lattice, double K) {
	for (int i = 0; i < warm_up_runs; ++i) {
		XySimulation(lattice, K);
	}
}
