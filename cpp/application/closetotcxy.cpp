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
		// std::ios_base::sync_with_stdio(false);

		const int dimension = 3;

		int nulltime = time(nullptr);
		srand((unsigned)nulltime + getpid());

		// Create data files
		auto windingnum_temp_file = GetUniqueFile("windingnum_tempXY");

		// How many different sizes of the simulation should run (L = 2^i)
		// for (auto& length : {4, 8, 16, 32, 64}) {
		for (auto& length : {4, 8}) {

			windingnum_temp_file << "L=" << length << ":\n";

			// for (double T : {.330, .331, .332, .333, .334, .335}) {
			// for (double T : {0.1, 0.35, 0.6}) {
			for (double T : {0.31, 0.32, 0.33, 0.34, 0.35}) {

				std::cout << "On length: " << length << "\n";
				std::cout << "On temperature: " << T << "\n";
;
				// Create a new graph
				int long seed = rand();
				Graph lattice(dimension, length, seed + getpid());

				// Bond strength J = 1
				double K = 1/T;

				double winding_number = 0;
				std::stringstream ss;
				ss << "xyl" << length << "t" << "0.35" << ".txt";
				auto start_data = WarmUpAndSaveOrReload(10'000 * length, lattice, K, ss.str());
				winding_number = start_data.winding_number;

				if ((T < 0.35) && (T >= 0.3)) {
					auto extra_data = WarmUp(1000 * length, lattice, K);
					winding_number += extra_data.winding_number;
				}

				int num_worms_started = 10'000;

				std::vector<double> winding_number_squared_vector;
				winding_number_squared_vector.reserve(num_worms_started);

				for (int i = 0; i < num_worms_started; ++i) {

					// How many new worms thrown before next measurement
					int refresh_state = 10;
					for (int j = 0; j < refresh_state; ++j) {
						auto res = XySimulation(lattice, K);
						winding_number += res.winding_number;
					}

					// Take measurement
					// NOTE: No longer dividing by 3 since only x direction
					winding_number_squared_vector.push_back(std::pow(winding_number, 2));

				}

				// Write to data files
				double avg_w_square = std::accumulate(winding_number_squared_vector.begin(),
									winding_number_squared_vector.end(), 0.0) / winding_number_squared_vector.size();
				double accum = 0.0;
				std::for_each (std::begin(winding_number_squared_vector), std::end(winding_number_squared_vector),
						[&](const double d) {
							accum += (d - avg_w_square) * (d - avg_w_square);
							});
				double stdev_w_square = sqrt(accum / (winding_number_squared_vector.size()-1));

				// Write to data files
				windingnum_temp_file << T;
				windingnum_temp_file << " " << avg_w_square;
				windingnum_temp_file << " " << stdev_w_square;
				windingnum_temp_file << " " << num_worms_started;
				windingnum_temp_file << "\n";

			}
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
