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
	unsigned length = 10;
	// 2D, 3D, ...
	const unsigned dimension = 2;

	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	// unsigned long mSeed = rand();

	// Produces a cluster with only two sites (impossible)
	// For: length: 10, dimension: 2
	// TODO: Solve this by debugging the cluster function probably
	unsigned long seed = 420974030;

	std::cout << "Seed is: " << seed << "\n";

	Graph lattice(dimension, length, seed);

	// Simulation
	try {

		// Get the first site for this simulation
		unsigned first_site = lattice.GetRandomSite();
		unsigned *null = nullptr;
		// Get some random neighbour to form the first link
		unsigned current_site = lattice.GetRandomNeighbour(first_site, null);
		// Track the previous site to avoid that the current turns 180 degrees
		unsigned previous_site = first_site;
		// Form the first link
		lattice.SwitchLinkBetween(first_site, current_site);

		// Initialize the correlation function
		std::unordered_map<unsigned, unsigned> correlation_func;
		UpdateCorrelationFunction(current_site, first_site, length, correlation_func);

		// Initialize the clusters map
		std::unordered_map<unsigned, std::vector<unsigned>> clusters;
		lattice.IndexClusters(clusters);

		// This will store all of the loop lengths
		std::vector<unsigned> loop_lengths;
		// This will store the average loop length according to Mats' notes
		double average_loop_length = 0;

		bool loop_formed = 0;
		while (!loop_formed) {
			// previous_site is passed as a pointer since when null, no site is excepted from being chosen
			unsigned next_site = lattice.GetRandomNeighbour(current_site, &previous_site);

			// std::cout << "Next site is: " << next_site << "\n";

			auto rand_num = lattice.GetRandomNum();
			if (IsAccepted(K, lattice.GetLink(current_site, next_site), rand_num)) {

				// std::cout << "Got accepted!" << "\n";
				// std::cout << "Switch link between sites: " << current_site << " and " << next_site << "\n";

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

					std::cout << "Cluster is:" << "\n";
					lattice.PrintClusters(clusters);
					
					std::cout << "Loop lengths are:" << "\n";
					std::cout << "(";
					for (auto l : loop_lengths) {
						std::cout << l << ", ";
					}
					std::cout << ")" << "\n";

				}
				UpdateCorrelationFunction(first_site, next_site, length, correlation_func);
			}
		}

	} catch(std::string error) {
		std::cout << error << "\n";
	}
}
