#include "Graph.h"
#include "utils.h"
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

/**
* @brief: Change value between site0 and site1 in mGraph so that 0 <-> 1
*
* @param: int site0
*       : int site1
*
* @return: void
*/
void Graph::SwitchLinkBetween(int site0, int site1) {
	std::cout << "Call to SwitchLinkBetween(site0 = " << site0 << ", site1 = " << site1 << ")" << "\n";
	// Check if sites are in mGraph
	if (IsInGraph(site0) && IsInGraph(site1)) {

		// Check if siteX is a neighbour to siteY
		// Will be:
		//     pair<int, bool> - If found
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
* @brief: Return the link between site0 and site1 if it exists. Otherwise throw std::string error.
*
* @param: int site0
*       : int site1
*
* @return: int
*/
int Graph::GetLink(int site0, int site1) {
	if (IsInGraph(site0) && IsInGraph(site1)) {
		if (AreNeighbours(site1, site0)) {
			return mGraph[site0].neighbours[site1];
		} else {
			// Error handling for site1 not neighbour to site0
			std::stringstream ss;
			ss << "Call to Graph::GetLink failed since "
				<< site1 << " is not a neighbour to " << site0 << "\n";
			throw ss.str();
		}
	} else {
		// Error handling for site0 not in mGraph
		std::stringstream ss;
		ss << "Call to Graph::GetLink failed since one of "
			<< site0 << " and " << site1 << " is not in mGraph" << "\n";
		throw ss.str();
	}
}

/**
* @brief: Check mGraph if site has neighbour
*
* @param: int site
*       : int neighbour
*
* @return: bool
*/
bool Graph::AreNeighbours(int site0, int site1) {
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

