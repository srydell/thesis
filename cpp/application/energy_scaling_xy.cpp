#include "Graph.h"
#include "utils.h"
#include "xyHelper.h"
#include "xyWorm.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>

int main(/*int argc, char** argv*/) {
	try {
		std::ios_base::sync_with_stdio(false);

		// Temperature
		const int dimension = 3;
		// TODO: Change this to actual Tc
		const double T = 0.33;
		// Bond strength J = 1
		const double K = 1/T;


		int nulltime = time(nullptr);
		srand((unsigned)nulltime + getpid());

		// Create data files
		auto energy_size_file = GetUniqueFile("energy_sizeXY");
		auto h_dimension_file = GetUniqueFile("h_dimensionXY");

		// How many different sizes of the simulation should run (L = 2^i)
		for (auto& length : {4, 8}) {
		// for (auto& length : {4, 8, 16, 32}) {
			std::cout << "On length: " << length << "\n";
			
			// Create a new graph
			int long seed = rand();
			Graph lattice(dimension, length, seed + getpid());

			// Always start on .35 and then move down with some small warmup
			std::stringstream ss;
			ss << "xyl" << length << "t" << T << ".txt";
			WarmUpAndSaveOrReload(10000 * length, lattice, K, ss.str());

			// Store all hausdorff_dimensions/total length for this L
			std::vector<double> hausdorff_dimensions;
			std::vector<double> total_worm_length;

			// Run the simulation for size from 2^2 to 2^max_length_exponent num_sim times
			int num_worms_started = 100;
			int num_sim = 100;
			for (int s = 0; s < num_sim; ++s) {
			
				// How many new worms started before next measurement
				int refresh_state = 10;
				for (int i = 0; i < num_worms_started; ++i) {
					WarmUp(refresh_state, lattice, K);

					// std::cout << "Warmed up. On worm number: " << i << "\n";

					// ------- Take measurement -------
					// This will store {Cluster index: [sites_in_cluster]}
					std::unordered_map<int, std::vector<int>> clusters;
					lattice.HKIndex(clusters);

					// This will store {Cluster index: Loop lengths}
					std::unordered_map<int, int> loop_lengths;
					UpdateLoopLengths(loop_lengths, clusters, lattice);

					std::unordered_map<int, std::vector<int>> blocks;
					lattice.DivideGraph(blocks);

					// Find the sites corresponding to the largest worm
					std::vector<int> largest_worm = clusters[GetMaximumMapIndex(loop_lengths)];
					std::unordered_map<int, int> sidelength_and_numoccupied;
					lattice.GetBoxDimension(blocks, sidelength_and_numoccupied, largest_worm);

					// Calculate the Hausdorff dimension
					for (auto& sl_and_no : sidelength_and_numoccupied) {
						// D_box = log(N_s) / log(1/s)
						double d_h = -1 * std::log(sl_and_no.second) / std::log(sl_and_no.first);
						hausdorff_dimensions.push_back(d_h);
					}

					// Calculate the total length = total energy
					int total_length = 0;
					for (auto& index_and_looplength : loop_lengths) {
						total_length += index_and_looplength.second;
					}
					total_worm_length.push_back(total_length);

					// std::cout << "Got the winding_number: " << res.winding_number / 3 << "\n";

				}

				// std::cout << "Taking measurements..." << "\n";

				energy_size_file << "L=" << length << ":\n";
				for (auto& e : total_worm_length) {
					energy_size_file << e << "\n";
				}

				h_dimension_file << "L=" << length << ":\n";
				for (auto& d_h : hausdorff_dimensions) {
					h_dimension_file << d_h << "\n";
				}

			}
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
