#include "Graph.h"
#include "IsingWorm.h"
#include "utils.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>

int main(/*int argc, char** argv*/) {
	try {

		// If no input
		// if (argc == 1) {
		// 	std::string error_no_data_file_name = "Please enter a name for a file to save data to";
		// 	throw error_no_data_file_name;
		// }
		// std::cout << "Got pass the argc" << "\n";

		// Bond strength
		// Temperature
		// Critical temperature for Ising
		// T_2D = 2 / (ln(1 + sqrt(2)))
		// T_3D = 4.515
		double T;
		const unsigned dimension = 3;
		if (dimension == 2) {
			T = 2.269185314213;
		} else {
			T = 4.515;
		}
		double J = 0.5;
		double K = J/T;

		int nulltime = time(nullptr);
		srand((unsigned)nulltime);

		// std::string final_sizes_filename = argv[0];
		// std::string final_sizes_filename = "final_sizes.txt";
		// std::string dimension_filename = "box_size_occupied.txt";

		// To store results
		// std::ofstream final_sizes_file;
		// std::ofstream dimensions_file;

		// Open all the datafiles
		// final_sizes_file.open(final_sizes_filename, std::ios_base::app);
		// dimensions_file.open(dimension_filename, std::ios_base::app);

		//unsigned max_length_exponent = 6;
		unsigned max_length_exponent = 3;

		std::vector<unsigned> max_loop_lengths;
		max_loop_lengths.reserve(max_length_exponent);

		// How many different sizes of the simulation should run (L = 2^i)
		for (unsigned i = 2; i < max_length_exponent; ++i) {
			// Create a new graph
			unsigned length = std::pow(2, i);
			unsigned long seed = rand();
			Graph lattice(dimension, length, seed);
		
			// std::cout << "Seed is: " << seed << "\n";

			// This will store (Cluster index: Loop lengths)
			std::unordered_map<unsigned, unsigned> loop_lengths;
		
			// Initialize the correlation function
			std::unordered_map<unsigned, unsigned> correlation_func;
		
			// Initialize the clusters map
			std::unordered_map<unsigned, std::vector<unsigned>> clusters;
		
			unsigned num_worms_started = 10000;
			// Run the simulation until it hopefully goes to equilibrium
			for (unsigned j = 0; j < num_worms_started; ++j) {

				// std::cout << "Worm number:" << j << "\n";

				IsingSimulation(lattice, correlation_func, length, K);
			}
		
			// Update indexing
			lattice.HKIndex(clusters);
		
			UpdateLoopLengths(loop_lengths, clusters, lattice);
		
			// Find the sites corresponding to the largest worm
			std::vector<unsigned> largest_worm = clusters[GetMaximumMapIndex(loop_lengths)];

			// Split up the graph in boxes of decreasing sizes
			std::unordered_map<unsigned, std::vector<unsigned>> blocks;
			lattice.DivideGraph(blocks);

			std::unordered_map<unsigned, unsigned> side_length_and_num_occupied;
			lattice.GetBoxDimension(blocks, side_length_and_num_occupied, largest_worm);

			// std::cout << "Box dimensions:" << "\n";
			std::cerr << "\n" << "L=" << length << ":\n";
			for (auto& sl_and_no : side_length_and_num_occupied) {
				std::cerr << sl_and_no.first << " ";
				std::cerr << sl_and_no.second << "\n";
			}
		
			// std::cout << *std::max_element(loop_lengths.begin(), loop_lengths.end()) << "\n";
			// After num_simulations the max length has converged and we can append it
			max_loop_lengths.push_back(loop_lengths[GetMaximumMapIndex(loop_lengths)]);
		
			// std::cout << "Finished with graph of size: " << std::pow(2, i) << "\n";

		}
		write_container(max_loop_lengths, std::cout, ' ');
		// Make a new row for the next run
		std::cout << '\n';

		// This will store the average loop length according to Mats' notes
		// double average_loop_length = GetAverageLoopLength(loop_lengths, K);

	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
