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

int main() {
	try {
		std::ios_base::sync_with_stdio(false);

		// Temperature
		const int dimension = 3;

		int nulltime = time(nullptr);
		srand((unsigned)nulltime + getpid());

		// How many different sizes of the simulation should run (L = 2^i)
		for (auto& length : {4, 8, 16, 32, 64}) {

			std::cout << "Dimension: " << dimension << "\n";
			std::cout << "On length: " << length << "\n";
			
			// Create a new graph
			int long seed = rand();
			Graph lattice(dimension, length, seed + getpid());

			// Bond strength J = 1
			// Tc \approx 0.333
			double T = 0.333;
			double K = 1/T;

			// Read in the closest file (T = 0.35)
			double winding_number = 0;
			double energy = 0;
			std::stringstream ss;
			ss << "xyl" << length << "t" << "0.35" << ".txt";
			auto start_data = WarmUpAndSaveOrReload(10'000 * length, lattice, K, ss.str());
			winding_number = start_data.winding_number;
			energy = start_data.energy;

			// Create the new file
			auto extra_data = WarmUp(1000 * length, lattice, K);
			winding_number += extra_data.winding_number;
			energy += extra_data.energy;

			std::stringstream ss2;
			ss2 << "xyl" << length << "t" << T << ".txt";
			SaveGraphToFile(ss2.str(), lattice, {winding_number, energy, 0.0});

		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
