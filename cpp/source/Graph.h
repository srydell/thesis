#ifndef GRAPH_H
#define GRAPH_H

#include "Site.h"
#include <vector>

class Graph {
public:
	// Constructor
	Graph (unsigned length, unsigned dimension);

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
	bool IsInVector(unsigned item, std::vector<unsigned> &vector_to_search);
	bool AllHaveIndex(unsigned index, std::vector<unsigned> &indices_to_search);
	void MoveToIndex(unsigned smallest_index, std::vector<unsigned> &local_cluster, std::unordered_map<unsigned, std::vector<unsigned>> &clusters);
	void PrintClusters(std::unordered_map<unsigned, std::vector<unsigned>> &to_print);
};

#endif
