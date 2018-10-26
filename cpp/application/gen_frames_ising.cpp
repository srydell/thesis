#include "Graph.h"
#include "utils.h"
#include "IsingWorm.h"
#include <algorithm>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>

void PrintVec(const std::string& name, const std::vector<int>& v) {
	std::cout << name << ":"<< "\n\t{";
	for (auto& value : v) {
		std::cout << value << ", ";
	}
	std::cout << "}" << "\n";
}

struct WNENS{
	WNENS(double winding_number, double energy, long double num_steps)
		: winding_number(winding_number), energy(energy), num_steps(num_steps) {};
	double winding_number;
	double energy;
	long double num_steps;
};

bool PosYDir(const int site0, const int site1, const int length) {
	int diff = site1 - site0;
	if ((diff == length) || (diff == -1 * length * (length-1))) {
		return 1;
	}
	return 0;
}

bool PosXDir(const int site0, const int site1, const int length) {
	int diff = site1 - site0;
	if ((diff == 1) || (diff == -1* (length-1))) {
		return 1;
	}
	return 0;
}

void SaveGraphToFile(const std::string& filename, Graph& lattice, const WNENS& data, bool debug) {
	std::ofstream file;
	file.open(filename, std::ios_base::out);

	// Write energy and winding_number
	{
		std::stringstream ss;
		ss << "Energy: " << data.energy << ", " << "Winding Number: " << data.winding_number;
		file << ss.str() << "\n";
	}

	// Special case for site = 0
	{
		std::vector<int> neighbours;
		lattice.GetLinkedNeighbours(0, neighbours);
		std::stringstream ss;
		if (neighbours.size() != 0) {
			ss << 0 << ":";
			for (auto& n : neighbours) {
				// NOTE: GetLink returns the physical link between 0, n
				ss << " " << std::abs(n) << "," << lattice.GetLink(0, std::abs(n));
			}
			file << ss.str() << "\n";

			if (debug) {
				PrintVec("Neighbours to 0", neighbours);
			}

		}
	}

	int l = lattice.GetLength();
	for (int i = 1; i < std::pow(l, lattice.GetDimension()); ++i) {
		std::vector<int> neighbours;
		lattice.GetLinkedNeighbours(i, neighbours);
		std::stringstream ss;
		// Only save links where the neighbour index > 0 to avoid double counting
		std::vector<int> pos_neighbours;
		std::copy_if(neighbours.begin(), neighbours.end(), std::back_inserter(pos_neighbours),
				[=](int n) { return (PosXDir(i, n, l) || PosYDir(i, n, l)); });

		if (pos_neighbours.size() != 0) {
			ss << i << ":";
			for (auto& n : pos_neighbours) {
				if (n > 0) {
					// NOTE: GetLink returns the physical link between i, n
					ss << " " << n << "," << lattice.GetLink(i, n);
				}
			}
			file << ss.str() << "\n";

			if (debug) {
				std::stringstream sdebug;
				sdebug << "Positive neighbours to " << i;
				PrintVec(sdebug.str(), pos_neighbours);
			}

		}
	}
}

int IsingSimulation(Graph & lattice, double K, int current_frame, int max_frame, bool save_graph, bool only_return_on_loop) {
	// Get the first site for this simulation
	int first_site = lattice.GetRandomSite();

	// Get some random neighbour to form the first link
	int current_site = first_site;

	bool loop_formed = 0;
	while (!loop_formed) {
		int next_site = lattice.GetRandomNeighbour(current_site);

		auto rand_num = lattice.GetRandomNum();
		if (IsAccepted(K, lattice.GetLink(current_site, next_site), rand_num)) {

			// Flip the weight between currentSite and nextSite
			lattice.SwitchLinkBetween(current_site, next_site);
			current_site = next_site;

			// If we have found a loop
			if (next_site == first_site) {
				loop_formed = 1;

				if (only_return_on_loop) {
					return current_frame;
				}

			}
		} else if (current_site == first_site) {
			// If the first site we tried failed, loop closed directly
			loop_formed = 1;

			if (only_return_on_loop) {
				return current_frame;
			}

		}

		// Always update current_frame
		bool debug = 0;
		// if (current_frame == 221) {
		// 	debug = 1;
		// }

		if (save_graph) {
			current_frame++;
			std::stringstream ss;
			ss << "animation/ising/" << current_frame;
			SaveGraphToFile(ss.str(), lattice, {0, 0, 0}, debug);
		}

		// Makes it possible to return if we reach a max frame
		// Else always return when there's a loop formed
		if (!only_return_on_loop) {
			if (current_frame > max_frame) {
				return current_frame;
			}
		}

	}
	return current_frame;
}

auto GetFile(const std::string& filename) -> decltype(auto) {
	std::ofstream file;
	std::stringstream ss;
	ss << "animation/ising/" <<  filename;
	file.open(ss.str());
	return file;
}
int main() {
	try {
		int nulltime = time(nullptr);
		srand((unsigned)nulltime);

		// Bond strength J = 1
		// Tc \approx 2.269185314213
		constexpr double T = 2.269185314213;
		constexpr double K = 1/T;
		constexpr int dimension = 2;
		constexpr int length = 8;

		// Create a new graph
		// const int long seed = rand();
		// std::cout << seed << "\n";
		// Good enough seed. Freeze frame at ~350
		constexpr int long seed = 1253900500;
		Graph lattice(dimension, length, seed);

		bool only_return_on_loop = 1;
		bool save_graph = 0;
		constexpr int warmup_runs = 100'000 * length;
		for (int i = 0; i < warmup_runs; ++i) {
			IsingSimulation(lattice, K, 0, 0, save_graph, only_return_on_loop);
		}
		std::cout << "Warmed up" << "\n";

		only_return_on_loop = 0;
		save_graph = 1;
		constexpr int number_of_frames = 350;
		int current_frame = 0;
		while (current_frame < number_of_frames) {
			std::cout << "Current frame: " << current_frame << "\n";
			current_frame = IsingSimulation(lattice, K, current_frame,
					number_of_frames, save_graph, only_return_on_loop);
		}

		// This will store {Cluster index: [sites_in_cluster]}
		std::unordered_map<int, std::vector<int>> clusters;
		lattice.HKIndex(clusters);

		// Split up the graph into boxes of decreasing sizes
		std::unordered_map<int, std::vector<int>> blocks;
		lattice.DivideGraph(blocks);

		auto blocks_file = GetFile("blocks");
		for (auto& key_list : blocks) {
			if (key_list.first != length) {
				blocks_file << "block_number=" << key_list.first << "\n";
				for (auto& site : key_list.second) {
					blocks_file << site << "\n";
				}
			}
		}

		// This will store {Cluster index: Loop lengths}
		std::unordered_map<int, int> loop_lengths;
		UpdateLoopLengths(loop_lengths, clusters, lattice);

		// Find the sites corresponding to the largest worm
		std::vector<int> largest_worm = clusters[GetMaximumMapIndex(loop_lengths)];

		auto largest_worm_file = GetFile("largest_worm");
		for (auto& s : largest_worm) {
			largest_worm_file << s << "\n";
		}

	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}