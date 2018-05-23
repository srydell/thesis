#ifndef GRAPH_H
#define GRAPH_H

#include "Site.h"
#include <vector>

class Graph
{
public:
	// Constructor
	Graph (int length, int dimension);

private:
	// Fields
	std::vector<Site> graph;

	// Functions
};

#endif
