#include "Graph.h"
#include "utils.h"
#include "main.h"
// #include <cmath>
#include <iostream>
#include <string>

int main(){
	// Bond strength
	double J = 0.5;
	// Temperature
	double T = 1;
	double K = J/T;
	std::cout << K << "\n";
	// Length of one side of the lattice
	int length = 4;
	// 2D, 3D, ...
	const int dimension = 2;
	Graph lattice(dimension, length);

	// g.PrintGraph();
	// Simulation
	try {

		// std::vector<unsigned> neighbours;

		// # Initialize starting site as some random [i, j, ...] within the gitter
		// firstSite = [random.randrange(0, maxX) for maxX in size]
		// # Get some random neighbour to form the first link
		// currentSite = getRandomNeighbour(firstSite, None, gitter)
		// switchLinkBetween(currentSite, firstSite, gitter)
		// # Track the previous site to avoid that the current turns 180 degrees
		// previousSite = firstSite
		// clusters = {}
		// indexClusters(clusters, gitter)
		// updateCorrFunc(firstSite, currentSite, corrFunction)

		// Get the first site for this simulation
		unsigned first_site = lattice.GetRandomSite();
		unsigned * null = NULL;
		unsigned current_site = lattice.GetRandomNeighbour(first_site, null);
		// lattice.SwitchLinkBetween(first_site, current_site);
		// Track the previous site to avoid that the current turns 180 degrees
		unsigned previous_site = first_site;
		std::cout << "Previous site is : " << previous_site << "\n";
		lattice.SwitchLinkBetween(first_site, current_site);

		std::unordered_map<unsigned, unsigned> correlation_func;
		UpdateCorrelationFunction(current_site, first_site, correlation_func);
		// std::unordered_map<unsigned, std::vector<unsigned>> clusters;
		// lattice.IndexClusters(clusters);

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
	int key = site0 - site1;
	key = (key < 0) ? key*(-1) : key;
	if (HasKey(key, correlation_func)) {
		correlation_func[key]++;
	} else {
		correlation_func[key] = 1;
	}
}
