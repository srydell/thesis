#ifndef GRAPH_H
#define GRAPH_H

#include "Site.h"
#include <vector>
#include <random>

class Graph {
public:
	// Constructor
	Graph (unsigned length, unsigned dimension);

	// Functions
	void SwitchLinkBetween(unsigned site0, unsigned site1);
	unsigned GetRandomNeighbour(unsigned site, unsigned* exceptSite);
	void GetLinkedNeighbours(unsigned site, std::vector<unsigned> &linked_neighbours);
	void IndexClusters(std::unordered_map<unsigned, std::vector<unsigned>> &clusters);
	void FindIndices(std::unordered_map<unsigned, std::vector<unsigned>> &clusters, std::vector<unsigned> &local_cluster, std::vector<unsigned> &indices);
	void PrintGraph();
	long double GetRandomNum();
	unsigned GetRandomSite();
private:
	// Fields
	std::vector<Site> mGraph;
	// Related to random numbers
	//Initialize with non-deterministic seeds
	unsigned long mSeed;
	//Mersenne Twister: Good quality random number generator
	std::mt19937 mRng; 
	// Type of random number distribution
	std::uniform_real_distribution<double> mUniformDist{std::uniform_real_distribution<double>(0.0, 1.0)};

	// Functions
	bool AreNeighbours(unsigned site0, unsigned site1);
	bool IsInGraph(unsigned site);
	bool IsInVector(unsigned item, std::vector<unsigned> &vector_to_search);
	bool AllHaveIndex(unsigned index, std::vector<unsigned> &indices_to_search);
	void MoveToIndex(unsigned smallest_index, std::vector<unsigned> &local_cluster, std::unordered_map<unsigned, std::vector<unsigned>> &clusters);
	void PrintClusters(std::unordered_map<unsigned, std::vector<unsigned>> &to_print);
};

#endif
