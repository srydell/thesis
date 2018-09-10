#include "Graph.h"
#include "utils.h"
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

/**
* @brief: Constructor of Graph. Populate mGraph with all neighbours according to periodic boundary conditions.
*
* @param: int dimension
*       : int length
*       : int long seed
*/
Graph::Graph(int dimension, int length, int long seed) {
	this->mDimension = dimension;
	this->mLength = length;
	this->mSeed = seed;

	// Initialize the vector with a reserve of length^dimension
	mGraph.reserve(std::pow(length, dimension));

	for (int index = 0; index < std::pow(length,dimension); ++index) {
		// emplace_back to avoid copying the Site after creation
		mGraph.emplace_back(index, length, dimension);
	}

	//Mersenne Twister: Good quality random number generator
	mRng.seed(mSeed);
}

/**
* @brief: Populate linked_neighbours with all neighbours to site that has a value that is not 0.
*
* @param: int site
*
* @return: void
*/
void Graph::GetLinkedNeighbours(int site, std::vector<int> &linked_neighbours) {
	// Check if site is in mGraph
	if (IsInGraph(site)) {
		// Add the site index if the link value is not 0
		for (auto& index_and_value : mGraph[site].neighbours) {
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
* @param: int site
*
* @return: int
*/
int Graph::GetRandomNeighbour(int site) {
	// rand_num is in (0, 1)
	double rand_num = GetRandomNum();

	// Check if site is in mGraph
	if (IsInGraph(site)) {

		double prob_of_choosing_site = 1.0 / mGraph[site].neighbours.size();
		// Will be added to prob_of_choosing_site until a site is chosen or we run out of sites to choose from
		double prob_increase_per_site = prob_of_choosing_site;

		for (auto& index_and_value : mGraph[site].neighbours) {
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
* @brief: Check if site can be found in mGraph
*
* @param: int site
*
* @return: bool
*/
bool Graph::IsInGraph(int site) {
	size_t temp_site = site;
	if (temp_site < mGraph.size())
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
	for (auto& site : mGraph) {
		ss << site.GetIndex() << " : { ";
		for (auto& neighbour : site.neighbours) {
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
* @return: int long
*/
long double Graph::GetRandomNum(){
	return mUniformDist(mRng);
}

/**
* @brief: Return a random site index
*
* @param: 
*
* @return: int
*/
int Graph::GetRandomSite() {
	double rand_num = GetRandomNum();
	int site_index = std::round(rand_num * (mGraph.size() - 1));

	return site_index;
}

/**
* @brief: Creates a vector from index with x, y, z, ... depending on dimension of the graph
*
* @param: int index
*
* @return: std::vector<int>
*/
std::vector<int> Graph::GetxyzConversion(int index) {
	std::vector<int> xyz;
	xyz.reserve(mDimension);
	for (int i = 0; i < mDimension; ++i) {
		xyz[i] = index % mLength;
		index = std::floor(index / mLength);
	}
	return xyz;
}
