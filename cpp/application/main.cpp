#include "Graph.h"
#include "IsingWorm.h"
#include "utils.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
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
		const unsigned dimension = 2;
		if (dimension == 2) {
			T = 2.269185314213;
		} else {
			T = 4.515;
		}
		double J = 0.5;
		double K = J/T;

		int nulltime = time(nullptr);
		srand((unsigned)nulltime);

		// Create data files
		
		std::ofstream susceptibility_file;
		std::string susceptibility_filename = "box_size_occupied";
		std::stringstream ss;
		ss <<  "box_size_occupied" << getpid() << ".txt";
		susceptibility_file.open(susceptibility_filename, std::ios_base::app);

		// unsigned max_length_exponent = 8;
		unsigned max_length_exponent = 3;

		std::vector<unsigned> max_loop_lengths;
		max_loop_lengths.reserve(max_length_exponent);

		std::vector<long double> susceptibility;
		susceptibility.reserve(max_length_exponent);

		// How many different sizes of the simulation should run (L = 2^i)
		for (unsigned i = 2; i < max_length_exponent; ++i) {
			// Create a new graph
			unsigned length = std::pow(2, i);
			unsigned long seed = rand();
			Graph lattice(dimension, length, seed + getpid());
		
			// std::cout << "Seed is: " << seed << "\n";

			// This will store {Cluster index: Loop lengths}
			std::unordered_map<unsigned, unsigned> loop_lengths;
		
			// Initialize the clusters map
			std::unordered_map<unsigned, std::vector<unsigned>> clusters;
		
			unsigned num_worms_started = 10000;
			// Store how many total steps are accepted
			long double num_steps = 0.0;
			// Run the simulation until it hopefully goes to equilibrium
			for (unsigned j = 0; j < num_worms_started; ++j) {

				// std::cout << "Worm number:" << j << "\n";

				num_steps += IsingSimulation(lattice, K);
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

			std::cerr << "L=" << length << ":\n";
			for (auto& sl_and_no : side_length_and_num_occupied) {
				std::cerr << sl_and_no.first << " ";
				std::cerr << sl_and_no.second << "\n";
			}
		
			// std::cout << *std::max_element(loop_lengths.begin(), loop_lengths.end()) << "\n";
			// After num_simulations the max length has converged and we can append it
			max_loop_lengths.push_back(loop_lengths[GetMaximumMapIndex(loop_lengths)]);

			// susc = 1 / T * sum_i(g(i)) = 1 / T * ((step taken)/(worms started))
			susceptibility.push_back(num_steps / num_worms_started / T);
		
			// std::cout << "Finished with graph of size: " << std::pow(2, i) << "\n";

		}
		write_container(max_loop_lengths, std::cout, ' ');
		write_container(susceptibility, susceptibility_file, ' ');
		// Make a new row for the next run
		std::cout << '\n';
		susceptibility_file << '\n';

		// This will store the average loop length according to Mats' notes
		// double average_loop_length = GetAverageLoopLength(loop_lengths, K);

	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
