#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <unordered_map>
#include <vector>

/**
* @brief: brief
*
* @param: std::unordered_map<std::string, int>& graph
*       : int i
*       : const std::string& s
*
* @return: void
*/
void addSite(std::unordered_map<std::string, int>& graph, const std::string& s, int i)
{
	graph.insert({s, i});
}

/**
* @brief: Parses string and adds the neighbouring sites to graph according to periodic boundary conditions
*
* @param: std::unordered_map<std::string, int>& neighbours
*       : const std::string& s
*       : int length
*
* @return: void
*/
void applyBoundaryCondition(std::unordered_map<std::string, int>& neighbours, const std::string& s, int length)
{
	// This stream will provide the dimensions of the site
	std::istringstream iss(s);
	std::vector<int> site;
	std::string token;
	// Put every comma separated value into site
	while (std::getline(iss, token, ',')) {
		if (!token.empty())
			// Convert the string to an int and put it into the vector
			site.push_back(std::stoi(token));
	}

	std::stringstream ss;
	// Create the neighbours
	char delimiter = '|';
	for (int index : site) {
		// Check if i/j+1 is on the border
		ss << i << ',' << (j+1)%L << delimiter;
		ss << (i+1)%L << ',' << j << delimiter;
		// Check if i/j-1 is on the border
		if ((j-1) == -1) {
			ss << i << ',' << L-1 << delimiter;
		} else {
			ss << i << ',' << j-1 << delimiter;
		}
		if ((i-1) == -1) {
			ss << L-1 << ',' << j << delimiter;
		} else {
			ss << i-1 << ',' << j << delimiter;
		}
	}
}

int main(){
	// This is what I will use. Use a StringStream to turn ints into strings to then be used as keys.
	std::unordered_map< std::string, std::unordered_map<std::string, int> > graph;

	// Weight of each site initally
	int startingWeight = 0;

	// Size
	int L = 10;

	// Initialize the string stream
	std::stringstream ss;
	// Initialize the current site
	std::string site, neighbour;
	std::unordered_map<std::string, int> neighbours;
	// What will separate the neighbours in the string stream
	char delimiter = '|';
	for (int i = 0; i < L; ++i) {
		for (int j = 0; j < L; ++j) {
			// Create "i,j"
			ss << i << ',' << j;

			// Write it to the site
			site = ss.str();

			ss.str(std::string());
			// Create the neighbours
			// Check if i/j+1 is on the border
			ss << i << ',' << (j+1)%L << delimiter;
			ss << (i+1)%L << ',' << j << delimiter;
			// Check if i/j-1 is on the border
			if ((j-1) == -1) {
				ss << i << ',' << L-1 << delimiter;
			} else {
				ss << i << ',' << j-1 << delimiter;
			}
			if ((i-1) == -1) {
				ss << L-1 << ',' << j << delimiter;
			} else {
				ss << i-1 << ',' << j << delimiter;
			}

			// std::cout << "Current site: " << site << std::endl;
			// while (std::getline(ss, neighbour, delimiter)) {
			// 	if (neighbour.empty()) {
			// 		ss.sync();
			// 		ss.str(std::string());
			// 		break;
			// 	}
			// 	std::cout << "This is the neighbour string: " << neighbour << std::endl;
			// }

			// Put i,j from ss into neighbours up until delimiter
			std::getline(ss, neighbour, delimiter);
			addSite(neighbours, neighbour, startingWeight);
			// neighbours.insert({ neighbour, startingWeight });
			// Put i,j from ss into neighbours up until delimiter
			std::getline(ss, neighbour, delimiter);
			neighbours.insert({ neighbour, startingWeight });
			// Put i,j from ss into neighbours up until delimiter
			std::getline(ss, neighbour, delimiter);
			neighbours.insert({ neighbour, startingWeight });
			// Put i,j from ss into neighbours up until delimiter
			std::getline(ss, neighbour, delimiter);
			neighbours.insert({ neighbour, startingWeight });

			graph.insert({ site, neighbours });

			// Clear so it can be used in this loop
			site.clear();
			ss.str(std::string());
			neighbours.clear();
		}
	}

	for (std::pair<std::string, std::unordered_map<std::string, int>> siteInfo : graph) {
		for (std::pair<std::string, int> neighbourInfo : siteInfo.second) {
			std::cout << "Site " << siteInfo.first << " has neighbours: " << neighbourInfo.first << " with weight " << neighbourInfo.second << std::endl;
		}
	}
}
