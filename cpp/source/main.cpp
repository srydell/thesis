#include "Graph.h"
#include "IsingWorm.h"
#include "utils.h"
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
	unsigned length = 4;
	// 2D, 3D, ...
	const unsigned dimension = 2;
	Graph lattice(dimension, length);

	// g.PrintGraph();
	// Simulation
	try {

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
		UpdateCorrelationFunction(current_site, first_site, length, correlation_func);

		std::unordered_map<unsigned, std::vector<unsigned>> clusters;
		lattice.IndexClusters(clusters);

		// TODO: Change this to vector of unsigned after debugging enough
		std::vector<unsigned> loop_lengths;
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
					average_loop_length = GetAverageLoopLength(loop_lengths, K);
					std::cout << "Average loop length is: " << average_loop_length << "\n";
				}
				UpdateCorrelationFunction(first_site, next_site, length, correlation_func);
			}
		}

	} catch(std::string error) {
		std::cout << error << "\n";
	}
}
