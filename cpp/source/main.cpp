#include "Graph.h"
#include "utils.h"
#include "main.h"
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
		// Get some random neighbour to form the first link
		unsigned current_site = lattice.GetRandomNeighbour(first_site, null);
		// Track the previous site to avoid that the current turns 180 degrees
		unsigned previous_site = first_site;
		std::cout << "Previous site is : " << previous_site << "\n";
		// Form the first link
		lattice.SwitchLinkBetween(first_site, current_site);

		std::unordered_map<unsigned, unsigned> correlation_func;
		UpdateCorrelationFunction(current_site, first_site, correlation_func);
		UpdateCorrelationFunction(1, 2, correlation_func);

		// std::unordered_map<unsigned, std::vector<unsigned>> clusters;
		// lattice.IndexClusters(clusters);

		// loopFormed = False
		// while not loopFormed:
		//     # Get potential next step (choose any neighbour exept previousSite)
		//     nextSite = getRandomNeighbour(site=currentSite, exceptSite=previousSite, graph=gitter)
		//     if isAccepted(K, currentSite, nextSite, gitter):
		//         # Flip the weight between currentSite and nextSite
		//         switchLinkBetween(currentSite, nextSite, gitter)
		//         previousSite = currentSite
		//         currentSite = nextSite
		//         if nextSite == firstSite:
		//             # Found a new loop
		//             loopFormed = True
		//             # Update indexing
		//             clusters = {}
		//             indexClusters(clusters, gitter)
		//             loopLengths = updateLoopLengths(clusters, gitter)
		//         # Update the correlation function when new site is accepted
		//         updateCorrFunc(firstSite, nextSite, corrFunction)

		// bool loop_formed = 0;
		// while (!loop_formed) {
		// 	unsigned next_site = lattice.GetRandomNeighbour(current_site, &previous_site);
		// 	std::cout << next_site << "\n";
		// 	loop_formed = 1;
		// 	// if (IsAccepted(K, current_site, next_site, link_between)) {
		// 	// 		std::cout << "hi" << "\n";
		// 	// 	}
		// 	}

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

	std::cout << "Call to UpdateCorrelationFunction" << "\n";
	std::cout << "Site input: " << site0 << ", " << site1 << "\n";

	// Get the absolute number
	int key = (site0 > site1) ? site0 - site1 : site1 - site0;
	std::cout << "key after abs: " << key << "\n";

	if (HasItem(key, correlation_func)) {
		std::cout << "Adding +1 to old key: " << key << "\n";
		correlation_func[key]++;
	} else {
		std::cout << "Adding +1 to new key: " << key << "\n";
		correlation_func[key] = 1;
	}
}
