#include "Graph.h"
#include "Site.h"
#include "utils.h"
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

/**
* @brief: Constructor of Graph. Populate mGraph with all neighbours according to periodic boundary conditions.
*
* @param: unsigned dim
*       : unsigned len
*/
Graph::Graph(unsigned dimension, unsigned length, unsigned long seed) {
	this->mDimension = dimension;
	this->mLength = length;
	this->mSeed = seed;

	// Initialize the vector with a reserve of length^dimension
	mGraph.reserve(std::pow(length, dimension));

	for (unsigned index = 0; index < std::pow(length,dimension); ++index) {
		// emplace_back to avoid copying the Site after creation
		mGraph.emplace_back(index, length, dimension);
	}

	//Mersenne Twister: Good quality random number generator
	mRng.seed(mSeed);

	// TODO: Check if this functionality is useful: knowing exactly which sites are on the border
	// Here is only how many there are but which could be implemented somewhere
	//
	// std::vector<unsigned> border_values;
	// // Reserve number of sites on the border
	// if (dimension == 2) {
	// 	border_values.reserve(4 * (length - 1));
	// } else if (dimension == 3) {
	// 	border_values.reserve((6 * length * length) - (12 * length) + 8);
	// }

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
* @brief: Return a random neighbour to site
*
* @param: unsigned site
*
* @return: unsigned
*/
unsigned Graph::GetRandomNeighbour(unsigned site) {
	// rand_num is in (0, 1)
	double rand_num = GetRandomNum();

	// Check if site is in mGraph
	if (IsInGraph(site)) {

		double prob_of_choosing_site = 1.0 / mGraph[site].neighbours.size();
		// Will be added to prob_of_choosing_site until a site is chosen or we run out of sites to choose from
		double prob_increase_per_site = prob_of_choosing_site;

		for (auto index_and_value : mGraph[site].neighbours) {
			// Check if we should choose this site
			if (rand_num < prob_of_choosing_site) {
				return index_and_value.first;
			}
			// Else we go to next site
			prob_of_choosing_site += prob_increase_per_site;
		}
	}
	// Error handling for site not in mGraph
	std::stringstream ss;
	ss << "Call to Graph::GetRandomNeighbour failed since "
		<< site << " is not in the graph." << "\n";
	throw ss.str();
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

/**
* @brief: Return a random number generated from Mersienne Twister
*
* @param: 
*
* @return: unsigned long
*/
long double Graph::GetRandomNum(){
	return mUniformDist(mRng);
}

/**
* @brief: Return a random site index
*
* @param: 
*
* @return: unsigned
*/
unsigned Graph::GetRandomSite() {
	double rand_num = GetRandomNum();
	unsigned site_index = std::round(rand_num * (mGraph.size() - 1));

	return site_index;
}

/**
* @brief: Return the link between site0 and site1 if it exists. Otherwise throw std::string error.
*
* @param: unsigned site0
*       : unsigned site1
*
* @return: bool
*/
bool Graph::GetLink(unsigned site0, unsigned site1) {
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
* @brief: Creates a vector from index with x, y, z, ... depending on dimension of the graph
*
* @param: unsigned index
*
* @return: std::vector<unsigned>
*/
std::vector<unsigned> Graph::GetxyzConversion(unsigned index) {
	std::vector<unsigned> xyz;
	xyz.reserve(mDimension);
	for (unsigned i = 0; i < mDimension; ++i) {
		xyz[i] = index % mLength;
		index = std::floor(index / mLength);
	}
	return xyz;
}
