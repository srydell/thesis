#ifndef GRAPH_H
#define GRAPH_H

#include "Site.h"
#include <memory>
#include <random>
#include <vector>

class Graph {
public:
	// Constructor
	Graph(int dimension, int length, int long seed);

	// Functions
	long double GetRandomNum();
	int GetLink(int site0, int site1);
	int GetRandomNeighbour(int site);
	int GetRandomSite();
	void FindIndices(std::unordered_map<int, std::vector<int>> &clusters, std::vector<int> &local_cluster, std::vector<int> &indices);
	void GetLinkedNeighbours(int site, std::vector<int> &linked_neighbours);
	void PrintGraph();
	void PrintClusters(std::unordered_map<int, std::vector<int>> &to_print);
	void SwitchLinkBetween(int site0, int site1);
	std::vector<int> GetxyzConversion(int index);
	void GetPercolatingIndices(std::unordered_map<int, std::vector<int>> const &clusters, std::vector<int> &percolating_indices);

	// Only for XY
	int GetSign(int site0, int site1);

	// IndexClusters
	void IndexClusters(std::unordered_map<int, std::vector<int>> &clusters);

	// Hoshen Kopelman
	void HKIndex(std::unordered_map<int, std::vector<int>> &clusters);

	// ClusterDimension
	void DivideGraph(std::unordered_map<int, std::vector<int>>& blocks);
	void GetBoxDimension(std::unordered_map<int, std::vector<int>>& blocks, std::unordered_map<int, int>& num_occupied_and_sidelength, std::vector<int>& structure);
private:
	// Fields
	std::vector<Site> mGraph;
	// Related to random numbers
	//Initialize with non-deterministic seeds
	int long mSeed;
	//Mersenne Twister: Good quality random number generator
	std::mt19937 mRng; 
	// Type of random number distribution
	std::uniform_real_distribution<double> mUniformDist{std::uniform_real_distribution<double>(0.0, 1.0)};
	int mLength;
	int mDimension;

	bool AreNeighbours(int site0, int site1);
	bool IsInGraph(int site);
	bool IsInVector(int item, std::vector<int> &vector_to_search);
	bool AllHaveIndex(int index, std::vector<int> &indices_to_search);
	void MoveToIndex(int smallest_index, std::vector<int> &local_cluster, std::unordered_map<int, std::vector<int>> &clusters);

	// ClusterDimension
	void DivideGraphRec(std::unordered_map<int, std::vector<int>>& blocks, int side_length, int start, int system_size);
	void AppendCurrentBox(int start, int side_length, std::vector<int>& current_box);
	void NextToAdd(std::vector<int>& to_add, int side_length);

	// Hoshen Kopelman
	void Union(int neighbour, int largest_label, std::vector<int> &local_cluster, std::unique_ptr<std::vector<bool>> &visited_clusters);
};

#endif
