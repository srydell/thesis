#include <iostream>
#include <map>
#include <set>
#include <string>
#include <sstream>
#include <unordered_map>

/**
* @brief: brief
*
* @param: std::string hello
*       : std::string site
*       : std::unordered_map< std::string
*       : std::unordered_map<std::string
*       : int> >
*       : int i
*       : int j
*
* @return: void
*/
// void applyBoundaryCondition(std::unordered_map<std::string, std::unordered_map<std::string, int> > graph, int i, int j, int L)
// {
// 	// Assume periodic boundary condition

// 	// Initialize unordered_map called neighbours
// 	std::unordered_map<std::string, int> neighbours;

// 	int rightI = i;
// 	int rightJ = (j + 1)%L;
// 	// right = (i, (j+1)%M)
// 	// up = ((i-1)%N, j)
// 	// left = (i, (j-1)%M)
// 	// down = ((i+1)%N, j)
// 	// neighbours = [right, up, left, down]

// 	// Create each neighbour

// 	// Add them to neighbours 

// 	// Return the neighbours
// }

int main(){
	// This is what I will use. Use a StringStream to turn ints into strings to then be used as keys.
	std::unordered_map< std::string, std::unordered_map<std::string, int> > graph;

	// Weight of each site initally
	int startingWeight = 0;

	// Size
	int L = 10;

	// std::string testSite = "1,2";
	// std::unordered_map<std::string, int> testNeighbours;
	// std::string n1 = "2,2";
	// std::string n2 = "1,3";
	// std::string n3 = "1,1";
	// std::string n4 = "0,2";
	// testNeighbours.insert({ n1, startingWeight });
	// testNeighbours.insert({ n2, startingWeight });
	// testNeighbours.insert({ n3, startingWeight });
	// testNeighbours.insert({ n4, startingWeight });

	// graph.insert({ testSite, testNeighbours });

	// Iterate Over the unordered_map and display elements
	// for (std::pair<std::string, int> element : testNeighbours)
		// std::cout << element.first << " :: " << element.second << std::endl;

	// Initialize the string stream
	std::stringstream ss;
	// Initialize the current site
	std::string site, neighbour;
	std::unordered_map<std::string, int> neighbours;
	// What will separate the neighbours in the string stream
	char delimiter = '|';
	for (int i = 0; i < L; ++i) {
		for (int j = 0; j < L; ++j) {
			// Clear so it can be used in this loop
			site.clear();
			ss.str(std::string());

			// Create "i,j"
			ss << i << ',' << j;

			// Write it to the site
			site = ss.str();

			ss.str(std::string());
			// Create the neighbours
			ss << i << ',' << (j+1)%L << '|';
			ss << i << ',' << (j-1)%L << '|';
			ss << (i+1)%L << ',' << j << '|';
			ss << (i-1)%L << ',' << j << '|';

			// Put i,j from ss into neighbours up until delimiter
			std::getline(ss, neighbour, delimiter);
			std::cout << "Current site: " << site << std::endl;
			// std::cout << "This is the neighbour string: " << neighbour << std::endl;
			// Put i,j from ss into neighbours up until delimiter
			std::getline(ss, neighbour, delimiter);
			std::cout << "This is the neighbour2 string: " << neighbour << std::endl;
			// Put i,j from ss into neighbours up until delimiter
			std::getline(ss, neighbour, delimiter);
			std::cout << "This is the neighbour3 string: " << neighbour << std::endl;
			// Put i,j from ss into neighbours up until delimiter
			std::getline(ss, neighbour, delimiter);
			std::cout << "This is the neighbour4 string: " << neighbour << std::endl;
			// right = (i, (j+1)%M)
			// up = ((i-1)%N, j)
			// left = (i, (j-1)%M)
			// down = ((i+1)%N, j)
			// neighbours = [right, up, left, down]

			// std::cout << site << std::endl;
			// applyBoundaryCondition(graph, i, j, L);
		}
	}
}
