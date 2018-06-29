#include "Graph.h"
#include "IsingWorm.h"
#include "utils.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

int main(){
	// Bond strength
	double J = 0.5;
	// Temperature
	// Critical temperature for Ising
	// 2 / (ln(1 + sqrt(2)))
	double T = 2.269185314213;
	double K = J/T;
	// Length of one side of the lattice
	unsigned length = std::pow(2, 4);
	// 2D, 3D, ...
	const unsigned dimension = 2;

	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	unsigned long seed = rand();

	// Produces a cluster with only two sites (impossible)
	// For: length: 10, dimension: 2
	// TODO: Solve this by debugging the cluster function probably
	// unsigned long seed = 420974030;
	// unsigned long seed = 1692706254;

	std::cout << "Seed is: " << seed << "\n";
	std::cout << "Length is: " << length << "\n";

	// Initialize the correlation function
	std::unordered_map<unsigned, unsigned> correlation_func;

	Graph lattice(dimension, length, seed);

	// This will store all of the loop lengths
	std::vector<unsigned> loop_lengths;

	// Initialize the clusters map
	std::unordered_map<unsigned, std::vector<unsigned>> clusters;
	lattice.IndexClusters(clusters);

	try {

		std::ofstream outfile;

		outfile.open("test.txt", std::ios_base::app);
		for (unsigned i = 0; i < 10; ++i) {
			// Create a new graph
			unsigned long seed = rand();
			Graph lattice(dimension, length, seed);

			// This will store all of the loop lengths
			std::vector<unsigned> loop_lengths;

			unsigned num_simulations = 10000;
			// unsigned num_simulations = 10;
			std::vector<unsigned> max_loop_lengths;
			max_loop_lengths.reserve(num_simulations);

			for (unsigned i = 0; i < num_simulations; ++i) {
				// Simulation
				IsingSimulation(lattice, correlation_func, length, K);

				// Update indexing
				lattice.IndexClusters(clusters);

				UpdateLoopLengths(loop_lengths, clusters, lattice);
				// std::cout << *std::max_element(loop_lengths.begin(), loop_lengths.end()) << "\n";
				max_loop_lengths.push_back(*std::max_element(loop_lengths.begin(), loop_lengths.end()));

			}
			std::cout << "Finished with graph number: " << i << "\n";
			write_container(max_loop_lengths, outfile, ' ');
			outfile << '\n';
		}

		// Update indexing
		lattice.IndexClusters(clusters);

		UpdateLoopLengths(loop_lengths, clusters, lattice);

		// This will store the average loop length according to Mats' notes
		double average_loop_length = GetAverageLoopLength(loop_lengths, K);

		std::cout << "Cluster is:" << "\n";
		lattice.PrintClusters(clusters);
		std::cout << "Average loop length is: " << average_loop_length << "\n";
		std::cout << "Loop lengths are:" << "\n";
		std::cout << "(";
		for (auto l : loop_lengths) {
			std::cout << l << ", ";
		}
		std::cout << ")" << "\n";

	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
