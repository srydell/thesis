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

/**
* @brief: Wrapper for calculating the Hausdorff dimension and pushing it to hausdorff_dimensions
*
* @param: std::vector<double>& hausdorff_dimensions
*         Graph& lattice
*
* @return: void
*/
void inline Calc_hausdorff_dim(std::unordered_map<double, std::vector<double>>& hausdorff_dimensions, Graph& lattice) {
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
		// hausdorff_dim = np.log(size_numocc[1]) / np.log(1 / box_relative_size)
		double rel_box_size = sl_and_no.first;
		rel_box_size /= lattice.GetLength();
		if ((rel_box_size != 1) && (sl_and_no.second != 0)) {
			double d_h = -1 * std::log(sl_and_no.second) / std::log(rel_box_size);
			hausdorff_dimensions[rel_box_size].push_back(d_h);

			// std::cout << "Hausdorff dimension: " << d_h << "\n";
			// std::cout << "Relative box size: " << rel_box_size << "\n";
			// std::cout << "Lattice length: " << lattice.GetLength() << "\n";
			// std::cout << "Sidelength: " << sl_and_no.first << "\n";
			// std::cout << "Number occupied: " << sl_and_no.second << "\n";
			// std::cout << "\n";

		}
	}
}

int main(/*int argc, char** argv*/) {
	try {
		// std::ios_base::sync_with_stdio(false);

		// Bond strength J = 1
		const int dimension = 3;
		const double T = 0.333;
		const double K = 1/T;

		int nulltime = time(nullptr);
		srand((unsigned)nulltime + getpid());

		// Create data files
		auto energy_size_file = GetUniqueFile("energy_sizeXY");
		auto h_dimension_file = GetUniqueFile("h_dimensionXY");

		// Specify formatting
		// <E> sigma_{E} Number_of_measurements
		energy_size_file << "// Format: avg(E) std(E) Num_measurements\n";
		// rel_box_size <D_h> sigma_{D_H} Number_of_measurements
		h_dimension_file << "// Format: Relative_box_size avg(D_H) std(D_H) Num_measurements\n";

		// How many different sizes of the simulation should run (L = 2^i)
		for (auto& length : {4, 8}) {
		// for (auto& length : {4, 8, 16, 32}) {
			std::cout << "On length: " << length << "\n";
			
			// Create a new graph
			int long seed = rand();
			Graph lattice(dimension, length, seed + getpid());

			double energy = 0;
			std::stringstream ss;
			ss << "xyl" << length << "t" << T << ".txt";
			auto start_data = WarmUpAndSaveOrReload(10'000 * length, lattice, K, ss.str());
			energy = start_data.energy;

			// Run the simulation for size from 2^2 to 2^max_length_exponent num_sim times
			int num_worms_started = 10'000;

			// {relative_box_size: [d_h0, d_h1, ...}, ...} for this L
			std::unordered_map<double, std::vector<double>> hausdorff_dimensions;
			std::vector<double> energy_vector;
			energy_vector.reserve(num_worms_started);

			for (int i = 0; i < num_worms_started; ++i) {

				// How many new worms thrown before next measurement
				int refresh_state = 10;
				auto res = WarmUp(refresh_state, lattice, K);
				energy += res.energy;

				// ------- Take measurement -------
				energy_vector.push_back(energy);
				Calc_hausdorff_dim(hausdorff_dimensions, lattice);

			}

			// std::cout << "Writing to data files..." << "\n";

			// Write to data files
			energy_size_file << "L=" << length << ":\n";
			double avg_energy = std::accumulate(energy_vector.begin(),
								energy_vector.end(), 0.0) / energy_vector.size();
			double accum = 0.0;
			std::for_each (std::begin(energy_vector), std::end(energy_vector), [&](const double d) {
					    accum += (d - avg_energy) * (d - avg_energy); });
			double stdev_energy = sqrt(accum / (energy_vector.size()-1));

			// <E> sigma_{E} Number_of_measurements
			energy_size_file << avg_energy << " " << stdev_energy << " " << energy_vector.size() << "\n";

			h_dimension_file << "L=" << length << ":\n";
			for (auto& relbox_and_dh : hausdorff_dimensions) {

				auto& dhs = hausdorff_dimensions[relbox_and_dh.first];

				double avg_dimension = std::accumulate(dhs.begin(),
									dhs.end(), 0.0) / dhs.size();
				accum = 0.0;
				std::for_each (std::begin(dhs), std::end(dhs), [&](const double d) {
						    accum += (d - avg_dimension) * (d - avg_dimension); });
				double stdev_dimension = sqrt(accum / (dhs.size()-1));

				// rel_box_size <D_h> sigma_{D_H} Number_of_measurements
				h_dimension_file << relbox_and_dh.first << " " <<
					avg_dimension << " " <<
					stdev_dimension << " " <<
					dhs.size() << "\n";
			}

		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
