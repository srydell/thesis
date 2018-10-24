#include "Graph.h"
#include "utils.h"
#include "xyHelper.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <unistd.h>
#include <unordered_map>

bool IsAccepted(double K, int link_between, int difference, long double &random_num) {
	// std::cout << "Call to IsAccepted(K = " << K << ", link_between = " << link_between << ", difference = " << difference << ", random_num = " << random_num << "\n";

    // Probability of being accepted
	int new_energy = std::pow(link_between + difference, 2);
	int old_energy = std::pow(link_between, 2);
    auto p = std::exp(- .5 * K * (new_energy - old_energy));

	// std::cout << "Probability of acceptance: p = " << p << "\n";

	return random_num < p;
}

int SignIfInXDirection(const int site0, const int site1, const int length) {
	int diff = site1 - site0;
	if ((diff == 1) || (diff == -1* (length-1))) {
		return 1;
	}
	if ((diff == -1) || (diff == +1* (length-1))) {
		return -1;
	}
	return 0;
}

void SaveGraphToFile(const std::string& filename, Graph& lattice, const WNENS& data) {
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
				// NOTE: GetLink returns the physical link between 0, n, therefore GetSign
				ss << " " << std::abs(n) << "," << lattice.GetSign(0, n) * lattice.GetLink(0, std::abs(n));
			}
			file << ss.str() << "\n";
		}
	}

	for (int i = 1; i < std::pow(lattice.GetLength(), lattice.GetDimension()); ++i) {
		std::vector<int> neighbours;
		lattice.GetLinkedNeighbours(i, neighbours);
		std::stringstream ss;
		// Only save links where the neighbour index > 0 to avoid double counting
		bool HasPositiveLinks = std::any_of(neighbours.begin(), neighbours.end(), [](int n) { return n > 0; });
		if ((neighbours.size() != 0) && HasPositiveLinks) {
			ss << i << ":";
			for (auto& n : neighbours) {
				if (n > 0) {
					// NOTE: GetLink returns the physical link between i, n, therefore GetSign
					ss << " " << n << "," << lattice.GetSign(i, n) * lattice.GetLink(i, n);
				}
			}
			file << ss.str() << "\n";
		}
	}
}

int XySimulation(Graph & lattice, double K, int current_frame) {
	// Get the first site for this simulation
	int first_site = lattice.GetRandomSite();
	int current_site = first_site;

	// Total link weights for this loop
	double winding_number = 0.0;
	// E = K/2 * sum(J^2)
	double energy = 0.0;

	// Total number of accepted steps
	long double num_steps = 0.0;

	bool loop_formed = 0;
	while (!loop_formed) {
		int next_site = std::abs(lattice.GetRandomNeighbour(current_site));

		int sign = lattice.GetSign(current_site, next_site);
		auto rand_num = lattice.GetRandomNum();
		auto link_between = lattice.GetLink(current_site, next_site);
		if (IsAccepted(K, link_between, sign, rand_num)) {

			num_steps++;
			winding_number += SignIfInXDirection(current_site, next_site, lattice.GetLength());

			// Energy changed during this
			int new_Jsquare = std::pow(link_between + sign, 2);
			int old_Jsquare = std::pow(link_between, 2);
			energy += K / 2 * (new_Jsquare - old_Jsquare);


			// Flip the weight between currentSite and nextSite
			lattice.SwitchLinkBetween(current_site, next_site);
			current_site = next_site;

			// If we have found a loop
			if (next_site == first_site) {
				loop_formed = 1;
			}

		} else if (current_site == first_site) {
			// If the first site we tried failed, loop closed directly
			loop_formed = 1;
		}

		// Always update current_frame
		current_frame++;
		std::stringstream ss;
		ss << "animation/xy/" << current_frame;
		SaveGraphToFile(ss.str(), lattice, {winding_number / lattice.GetLength(), energy, num_steps});
	}
	return current_frame;
}

int main() {
	try {
		std::ios_base::sync_with_stdio(false);

		// Temperature
		const int dimension = 3;

		int nulltime = time(nullptr);
		srand((unsigned)nulltime + getpid());

		// Bond strength J = 1
		// Tc \approx 0.333
		double T = 0.39;
		double K = 1/T;
        int length = 4;

		// Create a new graph
		int long seed = rand();
		Graph lattice(dimension, length, seed + getpid());

		int number_of_frames = 360 * 2;
		int current_frame = 0;
		while (current_frame < number_of_frames) {
			std::cout << "Current frame: " << current_frame << "\n";
			current_frame = XySimulation(lattice, K, current_frame);
		}

	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
