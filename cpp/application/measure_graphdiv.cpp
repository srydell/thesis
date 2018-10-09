#include "Graph.h"
#include "utils.h"
#include <chrono>
#include <cmath>
#include <iostream>
#include <string>
#include <unordered_map>

int main(){
	try {
		std::ios_base::sync_with_stdio(false);

		// Temperature
		const int dimension = 2;

		int nulltime = time(nullptr);
		srand((unsigned)nulltime + getpid());

		// Create data files
		auto time_file = GetUniqueFile("graph_dividing_time");

		// How many different sizes of the simulation should run (L = 2^i)
		for (auto& length : {4, 8, 16}) {
			std::cout << "On length: " << length << "\n";
			time_file << "n=" << std::pow(length, dimension) << ":\n";
			
			// Create a new graph
			int long seed = rand();
			Graph lattice(dimension, length, seed + getpid());

			// ------- Take measurement -------

			// Just to get the type
			auto t0 = std::chrono::high_resolution_clock::now();
			auto t1 = std::chrono::high_resolution_clock::now();
			auto accumulator = std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();

			int num_measurements = 1000;
			for (int i = 0; i < num_measurements; ++i) {
				auto t0 = std::chrono::high_resolution_clock::now();

				std::unordered_map<int, std::vector<int>> blocks;
				lattice.DivideGraph(blocks);

				auto t1 = std::chrono::high_resolution_clock::now();
				accumulator += std::chrono::duration_cast<std::chrono::milliseconds>(t1 - t0).count();
				std::cout << blocks[length][0] << "\n";
			}

			time_file << accumulator / num_measurements;
			time_file << "\n";
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
