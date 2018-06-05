#include "Graph.h"
#include "utils.h"
#include "main.h"
#include <iostream>
#include <string>
#include <unordered_map>

int main(){
	// Bond strength
	double J = 0.5;
	// Temperature
	double T = 1;
	double K = J/T;
	// Length of one side of the lattice
	int length = 4;
	// 2D, 3D, ...
	const int dimension = 2;
	Graph lattice(dimension, length);

	// g.PrintGraph();
	// Simulation
	try {

		// std::vector<unsigned> neighbours;

		// Get the first site for this simulation
		unsigned first_site = lattice.GetRandomSite();
		unsigned * null = NULL;
		// Get some random neighbour to form the first link
		unsigned current_site = lattice.GetRandomNeighbour(first_site, null);
		// Track the previous site to avoid that the current turns 180 degrees
		unsigned previous_site = first_site;
		std::cout << "Previous site is : " << previous_site << "\n";
		// Form the first link
		lattice.SwitchLinkBetween(first_site, current_site);

		std::unordered_map<unsigned, unsigned> correlation_func;
		UpdateCorrelationFunction(current_site, first_site, correlation_func);

		std::unordered_map<unsigned, std::vector<unsigned>> clusters;
		lattice.IndexClusters(clusters);

		// TODO: Change this to vector of unsigned after debugging enough
		std::vector<double> loop_lengths;
		double average_loop_length = 1;
		std::cout << average_loop_length << "\n";

		bool loop_formed = 0;
		while (!loop_formed) {
			unsigned next_site = lattice.GetRandomNeighbour(current_site, &previous_site);

			std::cout << "Next site is: " << next_site << "\n";

			// loop_formed = 1;
			auto rand_num = lattice.GetRandomNum();
			if (IsAccepted(K, lattice.GetLink(current_site, next_site), rand_num)) {

				std::cout << "Got accepted!" << "\n";
				std::cout << "Switch link between sites: " << current_site << " and " << next_site << "\n";

				// Flip the weight between currentSite and nextSite
				lattice.SwitchLinkBetween(current_site, next_site);
				previous_site = current_site;
				current_site = next_site;
				// If we have found a loop
				if (next_site == first_site) {
					std::cout << "Found a loop!" << "\n";
					loop_formed = 1;
					// Update indexing
					lattice.IndexClusters(clusters);
					// Update loop lengths
					UpdateLoopLengths(loop_lengths, clusters, lattice);
					average_loop_length = GetAverageLoopLength(loop_lengths);
				}
				UpdateCorrelationFunction(first_site, next_site, correlation_func);
			}
		}

	} catch(std::string error) {
		std::cout << error << "\n";
	}
}

/**
* @brief: Set the absolute difference in x value between site0 and site1 as a key if it doesn't already exist.
*         Then add +1 to the corresponding value.
*
* @param: unsigned site0
*       : unsigned site1
*       : std::unordered_map<unsigned, unsigned> &correlation_func
*
* @return: void
*/
void UpdateCorrelationFunction(unsigned site0, unsigned site1, std::unordered_map<unsigned, unsigned> &correlation_func) {
    // # add +1 to G(i-i0) for the open path from i0 to i
    // # NOTE: This has to be the absolute value,
    // #       otherwise it will be skewed toward the side with the largest number of sites.
	// TODO: Change this so it only adds keys in one direction
	//       Probably the highest for that dimension (y for 2D, z for 3D) since it is easier to extract

	std::cout << "Call to UpdateCorrelationFunction" << "\n";
	std::cout << "Site input: " << site0 << ", " << site1 << "\n";

	// Get the absolute number
	int key = (site0 > site1) ? site0 - site1 : site1 - site0;

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
	// TODO: Debug this function

    // Probability of being accepted
    auto p = std::pow(std::tanh(K), 1 - link_between);

	if (random_num < p) {
		return 1;
	} else {
		return 0;
	}
}

/**
* @brief: Get the average loop length weighted with tanh(L)
*
* @param: std::vector<double> &loop_lengths
*
* @return: double
*/
double GetAverageLoopLength(std::vector<double> &loop_lengths) {
	// TODO: Write this function
	std::cout << loop_lengths[0] << "\n";
	return 10;
}

/**
* @brief: For each cluster in clusters, find the length of that loop
*
* @param: std::vector<double> &loop_lengths
*       : std::unordered_map<unsigned, std::vector<unsigned> &clusters
*       : Graph &lattice
*
* @return: void
*/
void UpdateLoopLengths(std::vector<double> &loop_lengths, std::unordered_map<unsigned, std::vector<unsigned>> &clusters, Graph &lattice) {
	//TODO: Test this function

	std::cout << "Call to UpdateLoopLengths" << "\n";

	// Will be used to measure the length of all links from site
	std::vector<unsigned> linked_neighbours;
	for (auto index_and_sites : clusters) {
		// Start each cluster with zero length
		double current_length = 0;

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
