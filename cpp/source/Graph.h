#ifndef GRAPH_H
#define GRAPH_H

#include "Site.h"
#include <vector>

class Graph
{
public:
	// Constructor
	Graph (const unsigned length, const unsigned dimension);

	// Functions
	void switchLinkBetween(const unsigned site0, const unsigned site1);
private:
	// Fields
	std::vector<Site> graph;

	// Functions
};

#endif
