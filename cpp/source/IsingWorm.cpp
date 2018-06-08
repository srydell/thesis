#include "Graph.h"
#include "utils.h"
#include <iostream>
#include <string>
#include <unordered_map>

/**
* @brief: Set the absolute difference in x value between site0 and site1 as a key if it doesn't already exist.
*         Then add +1 to the corresponding value.
*
* @param: unsigned site0
*       : unsigned site1
*       : unsigned length
*       : std::unordered_map<unsigned, unsigned> &correlation_func
*
* @return: void
*/
void UpdateCorrelationFunction(unsigned site0, unsigned site1, unsigned length, std::unordered_map<unsigned, unsigned> &correlation_func) {
    // # add +1 to G(i-i0) for the open path from i0 to i
    // # NOTE: This has to be the absolute value,
    // #       otherwise it will be skewed toward the side with the largest number of sites.

	std::cout << "Call to UpdateCorrelationFunction" << "\n";
	std::cout << "Site input: " << site0 << ", " << site1 << "\n";

	// Get the x values for each site
	unsigned x_0 = site0 % length;
	unsigned x_1 = site1 % length;

	std::cout << "x value for site " << site0 << " is " << x_0 << "\n";
	std::cout << "x value for site " << site1 << " is " << x_1 << "\n";

	// Get the absolute number of the difference between the x values
	int key = (x_0 > x_1) ? x_0 - x_1 : x_1 - x_0;

	std::cout << "Correlation function is as: " << "\n";
	for (auto element : correlation_func) {
		std::cout << element.first << ": "<< element.second << "\n";
	}

	if (HasItem(key, correlation_func)) {
		std::cout << "Adding +1 to old key: " << key << "\n";
		correlation_func[key]++;
	} else {
		std::cout << "Adding +1 to new key: " << key << "\n";
		correlation_func[key] = 1;
	}
}

/**
* @brief: Check if the link between current_site and next_site in gitter is accepted
*
* @param: double K
*       : unsigned current_site
*       : unsigned next_site
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
* @param: std::vector<unsigned> &loop_lengths
*       : double const &K
*
* @return: double
*/
double GetAverageLoopLength(std::vector<unsigned> &loop_lengths, double const &K) {
	// TODO: Test this function

	//     tanh^L(K)
	double tanhK_to_l;

	//    \sum { l * tanh^l(K) }
	double sum_above = 0;

	//    \sum { tanh^l(K) }
	double sum_below = 0;

	for (unsigned l : loop_lengths) {
		tanhK_to_l = std::pow(std::tanh(K), l);
		sum_above += l * tanhK_to_l;
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
* @param: std::vector<unsigned> &loop_lengths
*       : std::unordered_map<unsigned, std::vector<unsigned> &clusters
*       : Graph &lattice
*
* @return: void
*/
void UpdateLoopLengths(std::vector<unsigned> &loop_lengths, std::unordered_map<unsigned, std::vector<unsigned>> &clusters, Graph &lattice) {
	//TODO: Test this function

	std::cout << "Call to UpdateLoopLengths" << "\n";

	// Will be used to measure the length of all links from site
	std::vector<unsigned> linked_neighbours;
	for (auto index_and_sites : clusters) {
		// Start each cluster with zero length
		unsigned current_length = 0;

		std::cout << "On index: " << index_and_sites.first << "\n";

		for (unsigned site : index_and_sites.second) {
			// Find the number of links going through site
			// NOTE: This will be double counting since if
			//       site0 is a linked neighbour to site1
			//       site1 is a linked neighbour to site0.
			lattice.GetLinkedNeighbours(site, linked_neighbours);
			current_length += linked_neighbours.size();

			std::cout << "Added site: " << site << "\n";
			std::cout << "Current length is now: " << current_length << "\n";

			// Reinitialize the vector to be used on the next site
			linked_neighbours.clear();
		}

		std::cout << "This cluster size is: " << current_length / 2 << "\n";

		// Divide by 2 to avoid double counting
		loop_lengths.push_back(current_length / 2);
	}
}
