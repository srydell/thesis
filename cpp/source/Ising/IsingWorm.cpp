#include "Graph.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <unordered_map>

/**
* @brief: Set the absolute difference in x value between site0 and site1 as a key if it doesn't already exist.
*         Then add +1 to the corresponding value.
*
* @param: int site0
*       : int site1
*       : int length
*       : std::unordered_map<int, int> &correlation_func
*
* @return: void
*/
void UpdateCorrelationFunction(int site0, int site1, int length, std::unordered_map<int, int> &correlation_func) {
    // # add +1 to G(i-i0) for the open path from i0 to i
    // # NOTE: This has to be the absolute value,
    // #       otherwise it will be skewed toward the side with the largest number of sites.

	// std::cout << "Call to UpdateCorrelationFunction" << "\n";
	// std::cout << "Site input: " << site0 << ", " << site1 << "\n";

	// Get the x values for each site
	int x_0 = site0 % length;
	int x_1 = site1 % length;

	// std::cout << "x value for site " << site0 << " is " << x_0 << "\n";
	// std::cout << "x value for site " << site1 << " is " << x_1 << "\n";

	// Get the absolute number of the difference between the x values
	int key = (x_0 > x_1) ? x_0 - x_1 : x_1 - x_0;

	// std::cout << "Correlation function is as: " << "\n";
	// for (auto element : correlation_func) {
	// 	std::cout << element.first << ": "<< element.second << "\n";
	// }

	if (MapHasItem(key, correlation_func)) {
		// std::cout << "Adding +1 to old key: " << key << "\n";
		correlation_func[key]++;
	} else {
		// std::cout << "Adding +1 to new key: " << key << "\n";
		correlation_func[key] = 1;
	}
}

/**
* @brief: Check if the link between current_site and next_site in gitter is accepted
*
* @param: double K
*       : int current_site
*       : int next_site
*       : bool link_between
*       : bool random_num
*
* @return: bool
*/
bool IsAccepted(double K, bool link_between, long double &random_num) {
    // Probability of being accepted
    auto p = std::pow(std::tanh(K), 1 - link_between);

	if (random_num < p) {
		return 1;
	} else {
		return 0;
	}
}

/**
* @brief: Get the average loop length weighted with tanh(K)
*         \sum { l * tanh^l(K) }
*         ----------------------
*           \sum { tanh^l(K) }
*
* @param: std::unordered_map<int, int> &loop_lengths
*       : double const &K
*
* @return: double
*/
double GetAverageLoopLength(std::unordered_map<int, int> &loop_lengths, double const &K) {
	// TODO: Test this function

	//    \sum { l * tanh^l(K) }
	double sum_above = 0;

	//    \sum { tanh^l(K) }
	double sum_below = 0;

	for (auto& index_and_loop_length : loop_lengths) {
		double tanhK_to_l = std::pow(std::tanh(K), index_and_loop_length.second);
		sum_above += index_and_loop_length.second * tanhK_to_l;
		sum_below += tanhK_to_l;
	}
	// \sum { l * tanh^l(K) }
	// ----------------------
	//   \sum { tanh^l(K) }
	return sum_above / sum_below;
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
*       : std::vector<int> correlation_func
*       : std::vector<int>> dimensions
*       : int length
*       : double K
*
* @return: long double
*/
long double IsingSimulation(Graph & lattice, double K) {
	// Get the first site for this simulation
	int first_site = lattice.GetRandomSite();

	// std::cout << "Got the first site: " << first_site << "\n";

	// Get some random neighbour to form the first link
	int current_site = lattice.GetRandomNeighbour(first_site);

	// std::cout << "Got the neighbour: " << current_site << "\n";

	// Form the first link
	lattice.SwitchLinkBetween(first_site, current_site);

	// Store the total number of accepted steps
	long double num_steps = 1.0;

	bool loop_formed = 0;
	while (!loop_formed) {
		int next_site = lattice.GetRandomNeighbour(current_site);

		// std::cout << "Next site is: " << next_site << "\n";

		auto rand_num = lattice.GetRandomNum();
		if (IsAccepted(K, lattice.GetLink(current_site, next_site), rand_num)) {
			num_steps++;

			// std::cout << "Got accepted!" << "\n";
			// std::cout << "Switch link between sites: " << current_site << " and " << next_site << "\n";

			// Flip the weight between currentSite and nextSite
			lattice.SwitchLinkBetween(current_site, next_site);
			current_site = next_site;
			// If we have found a loop
			if (next_site == first_site) {
				// std::cout << "Found a loop!" << "\n==========================\n\n";
				loop_formed = 1;

			}
		}
	}
	return num_steps;
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
	std::cout << "Ising warmup called" << "\n";
	for (int i = 0; i < warm_up_runs; ++i) {
		IsingSimulation(lattice, K);
	}
}
