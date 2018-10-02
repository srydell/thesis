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
		for (auto& length : {64, 128}) {

			std::cout << "Dimension: " << dimension << "\n";
			std::cout << "On length: " << length << "\n";
			
			// Create a new graph
			int long seed = rand();
			Graph lattice(dimension, length, seed + getpid());

			// Bond strength J = 1
			double T = 0.35;
			double K = 1/T;

			std::stringstream ss;
			ss << "xyl" << length << "t" << T << ".txt";
			std::string filename = ss.str();
			WarmUpAndSaveOrReload(10000 * length, lattice, K, filename);
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
