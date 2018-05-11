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
* @brief: Parse the string s and adds the neighbouring sites to neighbours according to periodic boundary conditions
*
* @param: std::unordered_map<std::string, int>& neighbours
*       : const std::string& s
*       : int length
*
* @return: void
*/
void applyBoundaryCondition(std::unordered_map<std::string, int>& neighbours, const std::string& s, int length, int startingWeight)
{
	// This stream will provide the dimensions of the site
	std::istringstream iss(s);
	std::vector<int> site;
	std::string token;
	std::cout << "Got the site: " << s << std::endl;
	// Put every comma separated value into site
	while (std::getline(iss, token, ',')) {
		if (!token.empty())
			// Convert the string to an int and put it into the vector
			site.push_back(std::stoi(token));
	}

	std::stringstream neighbourBuilder;
	// Create the neighbours
	// Index where boundary condition should be applied
	unsigned currentIndex = 0;
	// Index to be used between where boundary condition should be applied
	unsigned i = 0;
	for (int dimension : site) {
		std::cout << "Handling dimension index: " << dimension << std::endl;
		// Reset the changing index
		i = 0;
		// Add all indices before index into the stream
		while ((i < currentIndex) && (i < site.size())) {
			neighbourBuilder << site[i] << ',';
			i++;
		}
		// Here we handle the index where we should apply the boundary condition
		// On the last one we should have a delimiter to signify that we have looped through the whole site
		if (currentIndex == (site.size() - 1)) {
			neighbourBuilder << (site[currentIndex] + 1)%length;
			// Last index of the site, should not have ',' at the end
			if ((dimension - 1) == -1) {
				std::cout << "dimension - 1 is negative: " << dimension - 1 << std::endl;
				std::cout << "So adding: " << length - 1 << ',' << std::endl;
				neighbourBuilder << length - 1 << ',';
			} else {
				std::cout << "dimension - 1 is not negative: " << dimension - 1 << std::endl;
				std::cout << "So adding: " << (site[currentIndex] - 1)%length << std::endl;
				neighbourBuilder << (site[currentIndex] - 1)%length;
			}
		} else {
			// It is not the last index
			std::cout << "Adding: " << (site[currentIndex] + 1)%length << ',' << std::endl;
			neighbourBuilder << (site[currentIndex] + 1)%length << ',';
			// If we are at the bottom of that dimension
			if ((dimension - 1) == -1) {
				// Periodic boundary condition so we wrap around to the "top"
				neighbourBuilder << length - 1 << ',';
			} else {
				// dimension - 1 is within the graph
				neighbourBuilder << dimension - 1 << ',';
			}
			// Handle the indices after index
			while (i < site.size()) {
				if (i == (site.size() - 1)) {
					neighbourBuilder << i;
				} else {
					neighbourBuilder << i << ',';
				}
				i++;
			}
		}
		// We have checked all indices
		// so we can safely add the neighbour
		std::cout << "Adding neighbour: " << neighbourBuilder.str() << std::endl;
		neighbours.insert( {neighbourBuilder.str(), startingWeight} );
		// Clear the neighbourBuilder stream for the next neighbour
		neighbourBuilder.str(std::string());
		// Keep track of which index we are on
		currentIndex++;
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
	// char delimiter = '|';
	for (int i = 0; i < L; ++i) {
		for (int j = 0; j < L; ++j) {
			// Create "i,j"
			ss << i << ',' << j;

			// Write it to the site
			site = ss.str();

			applyBoundaryCondition(neighbours, site, L, startingWeight);
			graph.insert({ site, neighbours });

			// ss.str(std::string());
			// // Create the neighbours
			// // Check if i/j+1 is on the border
			// ss << i << ',' << (j+1)%L << delimiter;
			// ss << (i+1)%L << ',' << j << delimiter;
			// // Check if i/j-1 is on the border
			// if ((j-1) == -1) {
			// 	ss << i << ',' << L-1 << delimiter;
			// } else {
			// 	ss << i << ',' << j-1 << delimiter;
			// }
			// if ((i-1) == -1) {
			// 	ss << L-1 << ',' << j << delimiter;
			// } else {
			// 	ss << i-1 << ',' << j << delimiter;
			// }

			// std::cout << "Current site: " << site << std::endl;
			// while (std::getline(ss, neighbour, delimiter)) {
			// 	if (neighbour.empty()) {
			// 		ss.sync();
			// 		ss.str(std::string());
			// 		break;
			// 	}
			// 	std::cout << "This is the neighbour string: " << neighbour << std::endl;
			// }

			// // Put i,j from ss into neighbours up until delimiter
			// std::getline(ss, neighbour, delimiter);
			// addSite(neighbours, neighbour, startingWeight);
			// // neighbours.insert({ neighbour, startingWeight });
			// // Put i,j from ss into neighbours up until delimiter
			// std::getline(ss, neighbour, delimiter);
			// neighbours.insert({ neighbour, startingWeight });
			// // Put i,j from ss into neighbours up until delimiter
			// std::getline(ss, neighbour, delimiter);
			// neighbours.insert({ neighbour, startingWeight });
			// // Put i,j from ss into neighbours up until delimiter
			// std::getline(ss, neighbour, delimiter);
			// neighbours.insert({ neighbour, startingWeight });

			// graph.insert({ site, neighbours });

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
	std::cout << "The size of the graph is: " << graph.size() << std::endl;
	std::cout << "The size of the graph should be: " << L * L << std::endl;
}
