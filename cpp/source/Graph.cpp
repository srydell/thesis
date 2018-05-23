#include "Site.h"
#include <cmath>
#include <iostream>
#include <vector>

class Graph
{
public:
	// Constructor
	Graph (int dimension, int length);

private:
	// Fields
	std::vector<Site> graph;

	// Functions
};

// Constructor of Graph
Graph::Graph(int dimension, int length)
{
	// Initialize the vector with a reserve of length^dimension
	graph.reserve(std::pow(length, dimension));

	for (int index = 0; index < std::pow(length,dimension); ++index) {
		graph.push_back(Site(index, length));
	}
	std::cout << graph.size() << "\n";
}

