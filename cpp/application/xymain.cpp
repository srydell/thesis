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

		int nulltime = time(nullptr);
		srand((unsigned)nulltime + getpid());

		// Create data files
		auto windingnum_temp_file = GetUniqueFile("windingnum_tempXY");

		// Run the simulation for size from 2^2 to 2^max_length_exponent num_sim times
		int num_sim = 1;
		for (int multiplier = 0; multiplier < num_sim; ++multiplier) {
			// How many different sizes of the simulation should run (L = 2^i)
			for (auto& length : {4}) {
			// for (auto& length : {4, 8, 16, 32, 64, 128}) {

				double MIN_T = 2.15;
				double MAX_T = 2.3;
				double NUM_T = 3;
				for (double T = MIN_T; T <= MAX_T; T+=(MAX_T - MIN_T)/(NUM_T - 1)) {

					// std::cout << "On length: " << length << "\n";
					// std::cout << "On temperature: " << T << "\n";
					
					// Bond strength J = 1
					double K = 1/T;
					
					// Create a new graph
					int long seed = rand();
					Graph lattice(dimension, length, seed + getpid());
					
					// Store the winding number
					int winding_number_squared = 0;
					
					int num_worms_started = 10000;
					// Reach equilibrium
					WarmUp(num_worms_started * std::log2(length), lattice, K);
					
					int refresh_state = 10;
					for (int i = 0; i < num_worms_started; ++i) {
						// How many new worms started before next measurement
						WarmUp(refresh_state, lattice, K);
					
						// std::cout << "Warmed up. On worm number: " << i << "\n";
					
						// Take measurement
						WNandNS res = XySimulation(lattice, K);
						winding_number_squared += std::pow(res.winding_number / length, 2);
					}
					
					// std::cout << "Taking measurements..." << "\n";
					
					windingnum_temp_file << "L=" << length << ":\n";
					windingnum_temp_file << winding_number_squared / (num_worms_started * 3);
					windingnum_temp_file << " " << T;
					windingnum_temp_file << "\n";
				}
			}
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}