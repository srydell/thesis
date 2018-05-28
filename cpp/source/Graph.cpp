#include "Site.h"
#include <cmath>
#include <iostream>
#include <sstream>
#include <vector>

class Graph {
public:
	// Constructor
	Graph (unsigned dimension, unsigned length);

	// Functions
	void SwitchLinkBetween(unsigned site0, unsigned site1);
	unsigned GetRandomNeighbour(unsigned site, unsigned exceptSite, const double randNumber);
	void GetLinkedNeighbours(unsigned site, std::vector<unsigned> &linked_neighbours);
	void IndexClusters(std::unordered_map<unsigned, std::vector<unsigned>> &clusters);
	void FindIndices(std::unordered_map<unsigned, std::vector<unsigned>> &clusters, std::vector<unsigned> &local_cluster, std::vector<unsigned> &indices);
	void PrintGraph();
private:
	// Fields
	std::vector<Site> mGraph;

	// Functions
	bool AreNeighbours(unsigned site0, unsigned site1);
	bool IsInGraph(unsigned site);
	bool IsInVector(std::vector<unsigned> vector_to_search, unsigned item);
};

/**
* @brief: Constructor of Graph. Populate mGraph with all neighbours according to periodic boundary conditions.
*
* @param: unsigned dimension
*       : unsigned length
*/
Graph::Graph(unsigned dimension, unsigned length) {
	// Initialize the vector with a reserve of length^dimension
	mGraph.reserve(std::pow(length, dimension));

	for (unsigned index = 0; index < std::pow(length,dimension); ++index) {
		mGraph.push_back(Site(index, length));
	}
}

/**
* @brief: Change value between site0 and site1 in mGraph so that 0 <-> 1
*
* @param: unsigned site0
*       : unsigned site1
*
* @return: void
*/
void Graph::SwitchLinkBetween(unsigned site0, unsigned site1) {
	// Check if sites are in mGraph
	if (IsInGraph(site0) && IsInGraph(site1)) {

		// Check if siteX is a neighbour to siteY
		// Will be:
		//     pair<unsigned, bool> - If found
		//     neighbours.end()     - If not found

		// Get the ends of the list to check links against
		// auto end0 = mGraph[site0].neighbours.end();
		// auto end1 = mGraph[site1].neighbours.end();

		// auto link1 = mGraph[site1].neighbours.find(site0);
		if (AreNeighbours(site0, site1)) {
			// Get the link weights
			auto link0 = mGraph[site0].neighbours[site1];
			auto link1 = mGraph[site1].neighbours[site0];

			// Switch the links
			mGraph[site0].neighbours[site1] = link0 ? 0 : 1;
			mGraph[site1].neighbours[site0] = link1 ? 0 : 1;

		} else {
			// Error handling for siteX and siteY not neighbours
			std::stringstream ss;
			ss << "Call to Graph::SwitchLinkBetween failed since "
				<< site0 << " and " << site1 << " are not in neighbours." << "\n";
			throw ss.str();
		}

	} else {
		// Error handling for site not in mGraph
		std::stringstream ss;
		ss << "Call to Graph::SwitchLinkBetween failed since "
			<< site0 << " and/or " << site1 << " are not in mGraph." << "\n";
		throw ss.str();
	}
}

/**
* @brief: Populate linked_neighbours with all neighbours to site that has a value that is not 0.
*
* @param: unsigned site
*
* @return: void
*/
void Graph::GetLinkedNeighbours(unsigned site, std::vector<unsigned> &linked_neighbours) {
	// Check if site is in mGraph
	if (IsInGraph(site)) {
		// Add the site index if the link value is not 0
		for (auto index_and_value : mGraph[site].neighbours) {
			if (index_and_value.second != 0) {
				linked_neighbours.push_back(index_and_value.first);
			}
		}

	} else {
		// Error handling for site not in mGraph
		std::stringstream ss;
		ss << "Call to Graph::GetLinkedNeighbours failed since "
			<< site << " is not in the graph." << "\n";
		throw ss.str();
	}
}

/**
* @brief: Return a neighbour to site that is not exceptSite depending on randNumber input (double 0 to 1).
*
* @param: unsigned site
*       : unsigned exceptSite
*       : const double randNumber
*
* @return: unsigned
*/
unsigned Graph::GetRandomNeighbour(unsigned site, unsigned exceptSite, const double randNumber) {
	// Check if site is in mGraph
	if (IsInGraph(site)) {
	
		// Check that exceptSite is in mGraph[site].neighbours
		if (AreNeighbours(site, exceptSite)) {
			// If randNumber is less than probToChooseSite, the corresponding site is chosen
			// size - 1 since we have some exceptSite that can not be chosen
			double probToChooseSite = 1.0 / (mGraph[site].neighbours.size() - 1);
			// Will be added to probToChooseSite until a site is chosen or we run out of sites to choose from
			double probIncreasePerSite = probToChooseSite;

			// Add the site index if it is not exceptSite
			for (auto index_and_value : mGraph[site].neighbours) {
				// Make sure we do not return exceptSite
				if (index_and_value.first != exceptSite) {

					// Check if we should choose this site
					if (randNumber < probToChooseSite) {
						return index_and_value.first;
					// Else we go to next site
					} else {
						probToChooseSite += probIncreasePerSite;
					}
				}
			}
			// Error handling for Deafult: Unknown error
			// Something went wrong. Should have returned some site.
			std::stringstream ss;
			ss << "Call to Graph::GetRandomNeighbour failed somehow.\n"
				<< "Input was: "
				<< "Site: " << site << ", exceptSite: " << exceptSite << ", randNumber: " << randNumber << "\n"
				<< "probToChooseSite: " << probToChooseSite << "\n";
			throw ss.str();

		} else {
			// Error handling for siteX and siteY not neighbours
			std::stringstream ss;
			ss << "Call to Graph::GetRandomNeighbour failed since input site "
				<< site << " and exceptSite " << exceptSite << " are not in neighbours." << "\n";
			throw ss.str();
		}
	} else {
		// Error handling for site not in mGraph
		std::stringstream ss;
		ss << "Call to Graph::GetRandomNeighbour failed since "
			<< site << " is not in the graph." << "\n";
		throw ss.str();
	}
}

/**
* @brief: Check mGraph if site has neighbour
*
* @param: unsigned site
*       : unsigned neighbour
*
* @return: bool
*/
bool Graph::AreNeighbours(unsigned site0, unsigned site1) {
	// Try to find siteX in neighbours of siteY
	auto link0To1 = mGraph[site0].neighbours.find(site1);
	auto link1To0 = mGraph[site1].neighbours.find(site0);

	// Find the ends to check agains
	auto endOf0 = mGraph[site0].neighbours.end();
	auto endOf1 = mGraph[site1].neighbours.end();

	if ((link0To1 != endOf0) && (link1To0 != endOf1)) {
		return 1;
	}
	return 0;
}

/**
* @brief: Check if site can be found in mGraph
*
* @param: unsigned site
*
* @return: bool
*/
bool Graph::IsInGraph(unsigned site) {
	if (site < mGraph.size())
		return 1;
	else
		return 0;
}

/**
* @brief: Pretty print the contents of mGraph for debugging
*
* @param: 
*
* @return: void
*/
void Graph::PrintGraph() {
	std::stringstream ss;
	ss << "\nSite : { (n0, link0) (n0, link0)... }\n";
	for (Site site : mGraph) {
		ss << site.GetIndex() << " : { ";
		for (auto neighbour : site.neighbours) {
			ss << "(" << neighbour.first << ", " << neighbour.second << ") ";
		}
		ss << "}\n";
	}
	std::cout << ss.str();
}
