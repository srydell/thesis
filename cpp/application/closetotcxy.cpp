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
		// for (auto& length : {4, 8, 16, 32}) {
		for (auto& length : {4, 8}) {
			// for (double T : {.330, .331, .332, .333, .334, .335}) {
			// for (double T : {0.1, 0.35, 0.6}) {
			for (double T : {0.31, 0.32, 0.33, 0.34, 0.35}) {

				std::cout << "On length: " << length << "\n";
				std::cout << "On temperature: " << T << "\n";

				// Create a new graph
				int long seed = rand();
				Graph lattice(dimension, length, seed + getpid());

				// Bond strength J = 1
				double K = 1/T;

				// Warmup
				double winding_number = 0;
				for (int i = 0; i < 10'000 * length; ++i) {
					// Take measurement
					auto res = XySimulation(lattice, K);
					winding_number += res.winding_number;
				}

				double winding_number_squared = 0;
				int num_worms_started = 10'000;
				for (int i = 0; i < num_worms_started; ++i) {

					// How many new worms thrown before next measurement
					int refresh_state = 10;
					for (int j = 0; j < refresh_state; ++j) {
						auto res = XySimulation(lattice, K);
						winding_number += res.winding_number;
					}

					// Take measurement
					winding_number_squared += std::pow(winding_number / 3, 2);

				}

				// Write to data files
				windingnum_temp_file << "L=" << length << ":\n";
				windingnum_temp_file << winding_number_squared / num_worms_started;
				windingnum_temp_file << " " << T;
				windingnum_temp_file << "\n";

			}
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
