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

		// Create data files
		auto windingnum_temp_file = GetUniqueFile("warmup_winding");

		// How many different sizes of the simulation should run (L = 2^i)
		for (auto& length : {8}) {

			// Create a new graph
			int long seed = rand();
			Graph lattice(dimension, length, seed + getpid());
			std::string filename = "xyl8t2.txt";
			if (FileExists(filename)) {
				LoadGraphFromFile(filename, lattice);
			}
			
			// Bond strength J = 1
			double T = 2.0;
			double K = 1/T;

			windingnum_temp_file << "L=" << length << ":\n";

			std::cout << "On length: " << length << "\n";
			
			// Store the winding number
			double winding_number_squared = 0;

			int num_worms_started = 100'000;
			for (int i = 0; i < num_worms_started; ++i) {
				// Take measurement
				WNandNS res = XySimulation(lattice, K);
				winding_number_squared += std::pow(res.winding_number / 3, 2);

				windingnum_temp_file << res.winding_number / 3 << "\n";

			}
			SaveGraphToFile(filename, lattice);
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
