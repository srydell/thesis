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
		double J = 1;
		double K = J/T;

		int nulltime = time(nullptr);
		srand((unsigned)nulltime + getpid());

		// Create data files
		auto susceptibility_file = GetUniqueFile("susceptibility");
		auto worm_distribution_file = GetUniqueFile("worm_distribution");

		// unsigned max_length_exponent = 3;
		unsigned max_length_exponent = 8;

		// Run the simulation for each size 
		// 1 run takes about 22 seconds
		int num_sim = 1;
		for (int multiplier = 0; multiplier < num_sim; ++multiplier) {
			std::vector<unsigned> max_loop_lengths;
			max_loop_lengths.reserve(max_length_exponent);

			std::vector<long double> susceptibility;
			susceptibility.reserve(max_length_exponent);

			std::vector<long int> total_worm_length;
			total_worm_length.reserve(max_length_exponent);

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

				int num_worms_started = 10000;

				// Run the updates and throw away the results
				WarmUp(num_worms_started * i, lattice, K);

				// Store how many total steps are accepted
				long double num_steps = 0.0;

				// Run the simulation until it hopefully goes to equilibrium
				for (int j = 0; j < num_worms_started; ++j) {
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

				// After num_simulations the max length has converged and we can append it
				max_loop_lengths.push_back(loop_lengths[GetMaximumMapIndex(loop_lengths)]);

				// susc = 1 / T * sum_i(g(i)) = 1 / T * ((step taken)/(worms started))
				susceptibility.push_back(num_steps / (num_worms_started * T));

				worm_distribution_file << "L=" << length << ":\n";
				for (auto& index_and_ll : loop_lengths) {
					worm_distribution_file << index_and_ll.second;
				}
				worm_distribution_file << "\n";

			}
			write_container(max_loop_lengths, std::cout, ' ');
			write_container(susceptibility, susceptibility_file, ' ');
			// Make a new row for the next run
			std::cout << '\n';
			susceptibility_file << '\n';

			// This will store the average loop length according to Mats' notes
			// double average_loop_length = GetAverageLoopLength(loop_lengths, K);
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
