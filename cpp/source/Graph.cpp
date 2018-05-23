#include "Site.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

class Graph
{
public:
	// Constructor
	Graph (unsigned dimension, unsigned length);

	// Functions
	void SwitchLinkBetween(unsigned site0, unsigned site1);
	std::vector<unsigned> GetLinkedNeighbours(unsigned site);
private:
	// Fields
	std::vector<Site> graph;

	// Functions
};

// Constructor of Graph
Graph::Graph(const unsigned dimension, const unsigned length) {
	// Initialize the vector with a reserve of length^dimension
	graph.reserve(std::pow(length, dimension));

	for (unsigned index = 0; index < std::pow(length,dimension); ++index) {
		graph.push_back(Site(index, length));
	}
	std::cout << graph.size() << "\n";
}

void Graph::SwitchLinkBetween(const unsigned site0, const unsigned site1) {
	// Check if sites are in graph
	if (((site0 > 0) && site0 < graph.size()) && ((site1 > 0) && site1 < graph.size())) {

		// Check if siteX is a neighbour to siteY
		// Will be:
		//     pair<unsigned, bool> - If found
		//     neighbours.end()     - If not found
		auto link0 = graph[site0].neighbours.find(site1);
		auto link1 = graph[site1].neighbours.find(site0);

		// Get the ends of the list to check links against
		auto end0 = graph[site0].neighbours.end();
		auto end1 = graph[site1].neighbours.end();

		// auto link1 = graph[site1].neighbours.find(site0);
		if ((link0 != end0) && (link1 != end1)) {
			graph[site0].neighbours[site1] = link0->second ? 0 : 1;
			graph[site1].neighbours[site0] = link1->second ? 0 : 1;
		} else {
			// Error handling for siteX and siteY not neighbours
			std::stringstream ss;
			ss << "Call to Graph::SwitchLinkBetween failed since "
				<< site0 << " and " << site1 << " are not in neighbours." << "\n";
			throw ss.str();
		}

	} else {
		// Error handling for site not in graph
		std::stringstream ss;
		ss << "Call to Graph::SwitchLinkBetween failed since "
			<< site0 << " and/or " << site1 << " are not in graph." << "\n";
		throw ss.str();
	}
}

std::vector<unsigned> Graph::GetLinkedNeighbours(const unsigned site) {
	// Check if site is in graph
	if ((site > 0) && (site < graph.size())) {
		std::vector<unsigned> linked_neighbours;
		// Add the site index if the link value is not 0
		for (auto index_and_value : graph[site].neighbours) {
			if (index_and_value.second != 0) {
				std::cout << "Adding value " << index_and_value.first << "\n";
				linked_neighbours.push_back(index_and_value.first);
			}
		}
		return linked_neighbours;

	} else {
		// Error handling for site not in graph
		std::stringstream ss;
		ss << "Call to Graph::GetLinkedNeighbours failed since "
			<< site << " is not in graph." << "\n";
		throw ss.str();
	}
}
