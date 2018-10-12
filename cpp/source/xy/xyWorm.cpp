#include "Graph.h"
#include "utils.h"
#include "xyHelper.h"
#include <algorithm>
#include <iostream>
#include <regex>
#include <string>
#include <unordered_map>

/**
* @brief: Return +1 if site1 is a +x neighbour to site0, -1 if -x neighbour and 0 otherwise
*
* @param: const int site0
*       : const int site1
*       : const int length
*
* @return: int
*/
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
	// std::cout << "Call to IsAccepted(K = " << K << ", link_between = " << link_between << ", difference = " << difference << ", random_num = " << random_num << "\n";

    // Probability of being accepted
	int new_energy = std::pow(link_between + difference, 2);
	int old_energy = std::pow(link_between, 2);
    auto p = std::exp(- .5 * K * (new_energy - old_energy));

	// std::cout << "Probability of acceptance: p = " << p << "\n";

	return random_num < p;
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
* @brief: Run the simulation until at least one loop is formed and return winding_number, energy and the number of steps taken
*
* @param: Graph &lattice
*       : double K
*
* @return: WNENS
*/
WNENS XySimulation(Graph & lattice, double K) {
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
	}
	return {winding_number / lattice.GetLength(), energy, num_steps};
}

/**
* @brief: Run warm_up_runs number of updates and throw them away
*
* @param: int warm_up_runs
*       : Graph& lattice
*
* @return: WNENS
*/
WNENS WarmUp(int warm_up_runs, Graph& lattice, double K) {
	double winding_number = 0.0;
	double energy = 0.0;
	for (int i = 0; i < warm_up_runs; ++i) {
		auto res = XySimulation(lattice, K);
		winding_number += res.winding_number;
		energy += res.energy;
	}
	return {winding_number, energy, 0};
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
	// std::cout << "Switch link between " << site0 << " and " << site1 << " by: " << link << "\n";
	lattice.SetLinks(site0, site1, link);
}


/**
* @brief: Switch all links saved in filename and return the energy and winding number.
*
* @param: const std::string& filename
*       : Graph& lattice
*
* @return: WNENS
*/
WNENS LoadGraphFromFile(const std::string& filename, Graph& lattice) {
	std::ifstream file;
	file.open(filename, std::ios_base::in);

	std::regex energy_winding;
	{
		// Resulting regex:
		// Energy: <number>, Windgin Number: <number>
		std::stringstream ss;
		ss << "Energy: ";
		ss << "(-?\\d*\\.?\\d*)";
		ss << ", Winding Number: ";
		ss << "(-?\\d*\\.?\\d*)";
		energy_winding = ss.str();
	}

	double energy = 0;
	double winding_number = 0;
	bool found_energy_and_winding = 0;

	std::string line;
	while(std::getline(file, line)) {
		// std::cout << "Current line is: " << line << "\n";

		std::smatch matches;
		// Check for energy and winding_number
		// NOTE: There is only one such line, therefore checking if found
		if (!found_energy_and_winding) {
			if (std::regex_search(line, matches, energy_winding)) {
				std::stringstream(matches[1].str()) >> energy;
				std::stringstream(matches[2].str()) >> winding_number;
				found_energy_and_winding = 1;
				continue;
			}
		}

		std::regex sites_and_links;
		{
			// Find how many linked neighbours this site has
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

		if (std::regex_search(line, matches, sites_and_links)) {
			int current_site;
			if (matches.size() > 1) {
				std::stringstream(matches[1].str()) >> current_site;
			}
			for (size_t i = 2; i < matches.size(); ++i) {
				int neighbour, link;
				std::stringstream(matches[i].str()) >> neighbour;
				std::stringstream(matches[++i].str()) >> link;

				// std::cout << "\n================================" << "\n";

				// std::cout << "Setting links between: " << current_site << " and " << neighbour << "\n";
				SetLinks(current_site, neighbour, link, lattice);
			}
		}
	}

	return {winding_number, energy, 0};
}

/**
* @brief: Loop through lattice and save down the positive links. Except for site = 0 where it saves all links
*
* @param: const std::string& filename
*       : Graph& lattice
*
* @return: void
*/
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
		auto res = WarmUp(warm_up_runs, lattice, K);
		SaveGraphToFile(filename, lattice, res);
	}
}
