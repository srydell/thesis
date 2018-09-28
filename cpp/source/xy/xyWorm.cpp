#include "Graph.h"
#include "utils.h"
#include "xyHelper.h"
#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

/**
* @brief: Check if the link between current_site and next_site in gitter is accepted
*
* @param: double K
*       : int current_site
*       : int next_site
*       : int link_between
*       : int difference
*       : long double random_num
*
* @return: bool
*/
bool IsAccepted(double K, int link_between, int difference, long double &random_num) {
	std::cout << "Call to IsAccepted(K = " << K << ", link_between = " << link_between << ", difference = " << difference << ", random_num = " << random_num << "\n";

	// TODO: Check if this is correct. Probably with Mats
    // Probability of being accepted
	int new_energy = std::pow(link_between + difference, 2);
	int old_energy = std::pow(link_between, 2);
    auto p = std::exp(- .5 * K * (new_energy - old_energy));

	std::cout << "Probability of acceptance: p = " << p << "\n";

	if (random_num < p) {
		std::cout << "Accepted!" << "\n";
		return 1;
	} else {
		std::cout << "Rejected!" << "\n";
		return 0;
	}
}

/**
* @brief: For each cluster in clusters, find the length of that loop
*
* @param: std::unordered_map<int, int> &loop_lengths
*       : std::unordered_map<int, std::vector<int> &clusters
*       : Graph &lattice
*
* @return: void
*/
void UpdateLoopLengths(std::unordered_map<int, int> &loop_lengths, std::unordered_map<int, std::vector<int>> &clusters, Graph &lattice) {
	//TODO: Test this function

	// std::cout << "Call to UpdateLoopLengths" << "\n";

	// Will be used to measure the length of all links from site
	std::vector<int> linked_neighbours;
	for (auto index_and_sites : clusters) {
		// Start each cluster with zero length
		int current_length = 0;

		// std::cout << "On index: " << index_and_sites.first << "\n";

		for (int site : index_and_sites.second) {
			// Find the number of links going through site
			// NOTE: This will be double counting since if
			//       site0 is a linked neighbour to site1
			//       site1 is a linked neighbour to site0.
			lattice.GetLinkedNeighbours(site, linked_neighbours);
			current_length += linked_neighbours.size();

			// std::cout << "Added site: " << site << "\n";
			// std::cout << "That has the neighbours: " << "\n";
			// for (auto n : linked_neighbours) {
			// 	std::cout << n << " ";
			// }
			// std::cout << "\nCurrent length is now: " << current_length << "\n";

			// Reinitialize the vector to be used on the next site
			linked_neighbours.clear();
		}

		// std::cout << "This cluster size is: " << current_length / 2 << "\n";

		// Divide by 2 to avoid double counting
		loop_lengths[index_and_sites.first] = current_length / 2;
	}
}

/**
* @brief: Run the simulation until at least one loop is formed and return the number of steps taken
*
* @param: Graph &lattice
*       : double K
*
* @return: WNandNS
*/
WNandNS XySimulation(Graph & lattice, double K) {
	std::cout << "\n\nCall to XySimulation(lattice, K = " << K << ")" << "\n";

	// TODO: Fix this to be XY
	// Get the first site for this simulation
	int first_site = lattice.GetRandomSite();

	std::cout << "Got the first site: " << first_site << "\n";

	// Get some random neighbour to form the first link
	int current_site = std::abs(lattice.GetRandomNeighbour(first_site));

	std::cout << "Got the neighbour: " << current_site << "\n";

	// Store the total links for this loop (winding number)
	int sign = lattice.GetSign(first_site, current_site);
	double winding_number = sign;

	// Form the first link
	lattice.SwitchLinkBetween(first_site, current_site);

	// Store the total number of accepted steps
	long double num_steps = 1.0;

	bool loop_formed = 0;
	while (!loop_formed) {
		int next_site = std::abs(lattice.GetRandomNeighbour(current_site));

		std::cout << "(current_site, next_site) = (" << current_site << ", " << next_site << ")" << "\n";

		int sign = lattice.GetSign(current_site, next_site);
		auto rand_num = lattice.GetRandomNum();
		if (IsAccepted(K, lattice.GetLink(current_site, next_site), sign, rand_num)) {
			num_steps++;
			winding_number += sign;

			std::cout << "Got accepted!" << "\n";
			std::cout << "Switch link between sites: " << current_site << " and " << next_site << "\n";
			std::cout << "Call lattice.SwitchLinkBetween(" << current_site << ", " << sign << " * " <<  next_site << ");" << "\n";
			std::cout << "Current winding number: " << winding_number << "\n";

			// Flip the weight between currentSite and nextSite
			lattice.SwitchLinkBetween(current_site, next_site);
			current_site = next_site;

			// lattice.PrintGraph();

			// If we have found a loop
			if (next_site == first_site) {
				std::cout << "Found a loop!" << "\n==========================\n\n";
				loop_formed = 1;
			}

			// cout TEST cin
			std::string s;
			std::cin >> s;

		}
	}
	return {winding_number / lattice.GetLength(), num_steps};
}

/**
* @brief: Run warm_up_runs number of updates and throw them away
*
* @param: int warm_up_runs
*       : Graph& lattice
*
* @return: void
*/
void WarmUp(int warm_up_runs, Graph& lattice, double K) {
	for (int i = 0; i < warm_up_runs; ++i) {
		XySimulation(lattice, K);
	}
}

/**
* @brief: Help function to switch the link between site0 and site1 to have the value link
*
* @param: int site0
*       : int site1
*       : int link
*       : Graph& lattice
*
* @return: void
*/
void SetLinks(int site0, int site1, int link, Graph& lattice) {
	if (link < 0) {
		int tmp = site0;
		site0 = site1;
		site1 = tmp;
		link = -1 * link;
	}
	std::cout << "Switch link between " << site0 << " and " << site1 << " by: " << link << "\n";
	lattice.SetLinks(site0, site1, link);
}


/**
* @brief: Switch all links saved in filename.
*
* @param: const std::string& filename
*       : Graph& lattice
*
* @return: void
*/
void LoadGraphFromFile(const std::string& filename, Graph& lattice) {
	std::ifstream file;
	file.open(filename, std::ios_base::in);

	std::string line;
	while(std::getline(file, line)) {
		// std::cout << "Current line is: " << line << "\n";
		std::regex sites_and_links;
		// Find how many linked neighbours this site has
		{
			std::stringstream ss;
			ss << "(\\d+):";
			size_t n = std::count(line.begin(), line.end(), ',');
			for (size_t i = 0; i < n; ++i) {
				ss << " ";
				ss << "(-?\\d+),(-?\\d+)";
			}
			// std::cout << "Got the regex: " << ss.str() << "\n";
			// Create the resulting regex
			sites_and_links = ss.str();
		}

		std::smatch matches;
		if (std::regex_search(line, matches, sites_and_links)) {
			int current_site;
			if (matches.size() > 1) {
				std::stringstream(matches[1].str()) >> current_site;
			}
			for (size_t i = 2; i < matches.size(); ++i) {
				// TODO: Actually switch the sites
				int neighbour, link;
				std::stringstream(matches[i].str()) >> neighbour;
				std::stringstream(matches[++i].str()) >> link;

				// std::cout << "\n================================" << "\n";

				// std::cout << "Setting links between: " << current_site << " and " << neighbour << "\n";
				SetLinks(current_site, neighbour, link, lattice);
			}
		}
	}
}

/**
* @brief: Loop through lattice and save down the positive links. Except for site = 0 where it saves all links
*
* @param: const std::string& filename
*       : Graph& lattice
*
* @return: void
*/
void SaveGraphToFile(const std::string& filename, Graph& lattice) {
	std::ofstream file;
	file.open(filename, std::ios_base::out);
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

/**
* @brief: Check if the system has been saved before as a warmed up state, or warm it up and save it
*
* @param: int warm_up_runs
*       : Graph& lattice
*       : double K
*       : std::string& filename
*
* @return: void
*/
void WarmUpAndSaveOrReload(int warm_up_runs, Graph& lattice, double K, const std::string& filename) {
	if (FileExists(filename)) {
		LoadGraphFromFile(filename, lattice);
	} else {
		WarmUp(warm_up_runs, lattice, K);
		SaveGraphToFile(filename, lattice);
	}
}
