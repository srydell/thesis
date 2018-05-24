#ifndef GRAPH_H
#define GRAPH_H

#include "Site.h"
#include <vector>

class Graph
{
public:
	// Constructor
	Graph (unsigned length, unsigned dimension);

	// Functions
	void SwitchLinkBetween(unsigned site0, unsigned site1);
	unsigned GetRandomNeighbour(unsigned site, unsigned exceptSite, const double randNumber);
	std::vector<unsigned> GetLinkedNeighbours(unsigned site);
private:
	// Fields
	std::vector<Site> mGraph;

	// Functions
	bool AreNeighbours(unsigned site0, unsigned site1);
	bool IsInGraph(unsigned site);
};

#endif
