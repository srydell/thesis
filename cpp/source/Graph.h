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
	void SwitchLinkBetween(const unsigned site0, const unsigned site1);
	std::vector<unsigned> GetLinkedNeighbours(unsigned site);
private:
	// Fields
	std::vector<Site> graph;

	// Functions
};

#endif
