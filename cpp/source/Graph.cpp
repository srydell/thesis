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
	std::vector<unsigned> GetLinkedNeighbours(unsigned site);
private:
	// Fields
	std::vector<Site> mGraph;

	// Functions
	bool HasNeighbour(unsigned site, unsigned neighbour);
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
	// std::cout << mGraph.size() << "\n";
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
	if (((site0 > 0) && site0 < mGraph.size()) && ((site1 > 0) && site1 < mGraph.size())) {

		// Check if siteX is a neighbour to siteY
		// Will be:
		//     pair<unsigned, bool> - If found
		//     neighbours.end()     - If not found
		auto link0 = mGraph[site0].neighbours.find(site1);
		auto link1 = mGraph[site1].neighbours.find(site0);

		// Get the ends of the list to check links against
		auto end0 = mGraph[site0].neighbours.end();
		auto end1 = mGraph[site1].neighbours.end();

		// auto link1 = mGraph[site1].neighbours.find(site0);
		if ((link0 != end0) && (link1 != end1)) {
			mGraph[site0].neighbours[site1] = link0->second ? 0 : 1;
			mGraph[site1].neighbours[site0] = link1->second ? 0 : 1;
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
* @brief: Return all neighbours to site that has a value that is not 0.
*
* @param: unsigned site
*
* @return: std::vector<unsigned>
*/
std::vector<unsigned> Graph::GetLinkedNeighbours(unsigned site) {
	// Check if site is in mGraph
	if ((site > 0) && (site < mGraph.size())) {
		std::vector<unsigned> linked_neighbours;
		// Add the site index if the link value is not 0
		for (auto index_and_value : mGraph[site].neighbours) {
			if (index_and_value.second != 0) {
				// std::cout << "Adding value " << index_and_value.first << "\n";
				linked_neighbours.push_back(index_and_value.first);
			}
		}
		return linked_neighbours;

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
	if ((site > 0) && (site < mGraph.size())) {
		// Check that exceptSite is in mGraph[site].neighbours
		auto linkToExceptSite = mGraph[site].neighbours.find(exceptSite);
		auto endOfNeighbours = mGraph[site].neighbours.end();
	
		if ((linkToExceptSite != endOfNeighbours)) {
			// If randNumber is less than probToChooseSite, the corresponding site is chosen
			// size - 1 since we have some exceptSite that can not be chosen
			double probToChooseSite = 1.0 / (mGraph[site].neighbours.size() - 1);
			// Will be added to probToChooseSite until a site is chosen or we run out of sites to choose from
			double probIncreasePerSite = probToChooseSite;

			std::cout << "Input site is: " << site << "\n";
			std::cout << "Input exceptSite is: " << exceptSite << "\n";
			std::cout << "Input random number is: " << randNumber << "\n";

			// Add the site index if it is not exceptSite
			for (auto index_and_value : mGraph[site].neighbours) {
				// Make sure we do not return exceptSite
				if (index_and_value.first != exceptSite) {

					std::cout << "probToChooseSite is: " << probToChooseSite << "\n";
					std::cout << "Handling site: " << index_and_value.first << "\n";

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
bool Graph::HasNeighbour(unsigned site, unsigned neighbour)
{
	// Check that exceptSite is in mGraph[site].neighbours
	auto linkToNeighbour = mGraph[site].neighbours.find(neighbour);
	auto endOfNeighbours = mGraph[site].neighbours.end();

	if (linkToNeighbour != endOfNeighbours) {
		return 1;
	}
	return 0;
}
