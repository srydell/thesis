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
		const double T = 0.33;
		// Bond strength J = 1
		const double K = 1/T;


		int nulltime = time(nullptr);
		srand((unsigned)nulltime + getpid());

		// Create data files
		auto energy_size_file = GetUniqueFile("energy_sizeXY");

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

			// Run the simulation for size from 2^2 to 2^max_length_exponent num_sim times
			int num_worms_started = 100;
			int num_sim = 100;
			for (int s = 0; s < num_sim; ++s) {
			
				// How many new worms started before next measurement
				int refresh_state = 10;
				for (int i = 0; i < num_worms_started; ++i) {
					WarmUp(refresh_state, lattice, K);

					// std::cout << "Warmed up. On worm number: " << i << "\n";

					// Take measurement

					// std::cout << "Got the winding_number: " << res.winding_number / 3 << "\n";

				}

				// std::cout << "Taking measurements..." << "\n";

				energy_size_file << "L=" << length << ":\n";
				energy_size_file << winding_number_squared / num_worms_started;
				energy_size_file << " " << T;
				energy_size_file << "\n";

			}
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
