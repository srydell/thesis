#include "Graph.h"
#include "utils.h"
#include "xyWorm.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>

void RemoveAllBut(Graph& lattice, const std::vector<int>& exceptv) {
	int l = lattice.GetLength();
	int d = lattice.GetDimension();
	for (int i = 0; i < std::pow(l, d); ++i) {
		// If not in vector
		if (std::find(exceptv.begin(), exceptv.end(), i) == exceptv.end()) {
			std::vector<int> neighbours;
			lattice.GetLinkedNeighbours(i, neighbours);
			for (auto& n : neighbours) {
				lattice.SetLinks(i, std::abs(n), 0);
			}
		}
	}
}

int main() {
	try {
		int nulltime = time(nullptr);
		srand((unsigned)nulltime);

		// Bond strength J = 1
		constexpr double T = 0.333;
		constexpr double K = 1 / T;
		constexpr int dimension = 3;
		constexpr int length = 64;
		// constexpr int length = 8;
		//
		// Create a new graph
		// const int long seed = rand();
		// std::cout << seed << "\n";
		// Good enough seed
		constexpr int long seed = 1253900500;
		Graph lattice(dimension, length, seed);

		std::string filename_in = "xyl64t0.333.txt";
		WarmUpAndSaveOrReload(1, lattice, K, filename_in);

		std::cout << "Warmed up!" << "\n";

		// This will store {Cluster index: [sites_in_cluster]}
		std::unordered_map<int, std::vector<int>> clusters;
		lattice.HKIndex(clusters);

		std::cout << "HK ran fine." << "\n";

		// This will store {Cluster index: Loop lengths}
		std::unordered_map<int, int> loop_lengths;
		UpdateLoopLengths(loop_lengths, clusters, lattice);

		std::cout << "Updated Loop lengths" << "\n";

		// Find the sites corresponding to the largest worm
		std::vector<int> largest_worm = clusters[GetMaximumMapIndex(loop_lengths)];

		std::cout << "Got the maximum worm" << "\n";

		RemoveAllBut(lattice, largest_worm);

		std::cout << "Removed all but the maximum worm" << "\n";

		std::string filename_out = "animation/xy/largest_cluster/largest_cluster";
		SaveGraphToFile(filename_out, lattice, {0, 0, 0});

		std::cout << "Saved to file: " << filename_out << "\n";

	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
