#include "Graph.h"
#include "Site.h"
#include "catch.hpp"
#include "cmath"
#include "utils.h"
#include "xyWorm.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

TEST_CASE( "4x4 blocks are the correct size", "[ClusterDimension]" ) {
	int dimension = 2;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);
	int long seed = rand();

	Graph lattice = Graph(dimension, length, seed);
	std::unordered_map<int, std::vector<int>> blocks;

	lattice.DivideGraph(blocks);
	SECTION( "The block with system size as side length should contain all sites" )
	for (int i = 0; i < std::pow(length, dimension); ++i) {
		std::stringstream ss;
		ss << "Is site number: " << i << " in the block with side length same as system size?";
		INFO( ss.str() );

		bool has_site = std::find(blocks[length].begin(), blocks[length].end(), i) != blocks[length].end();
		REQUIRE( has_site );
	}

	SECTION( "The unsorted map blocks should contain blocks of sizes 4 and 2" )
	// Blocks should contain blocks with side length 2, 4
	REQUIRE( MapHasItem(4, blocks) );
	REQUIRE( MapHasItem(2, blocks) );

	INFO( "Block with size 4 has all the indices sorted" );
	std::vector<int> block_4 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	REQUIRE( blocks[4] == block_4 );

	INFO( "Block with size 2 has all the indices sorted two and two" );
	std::vector<int> block_2 = {0, 1, 4, 5, 8, 9, 12, 13, 2, 3, 6, 7, 10, 11, 14, 15};
	REQUIRE( blocks[2] == block_2 );
}

TEST_CASE( "8x8 blocks are the correct size", "[ClusterDimension]" ) {
	int dimension = 2;
	int length = 8;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);
	int long seed = rand();

	Graph lattice = Graph(dimension, length, seed);
	std::unordered_map<int, std::vector<int>> blocks;

	lattice.DivideGraph(blocks);
	SECTION( "The block with system size as side length should contain all sites" )
	for (int i = 0; i < std::pow(length, dimension); ++i) {
		std::stringstream ss;
		ss << "Is site number: " << i << " in the block with side length same as system size?";
		INFO( ss.str() );

		bool has_site = std::find(blocks[length].begin(), blocks[length].end(), i) != blocks[length].end();
		REQUIRE( has_site );
	}

	SECTION( "The unsorted map blocks should contain blocks of sizes 8, 4 and 2" )
	// Blocks should contain blocks with side length 2, 4, 8
	REQUIRE( MapHasItem(2, blocks) );
	REQUIRE( MapHasItem(4, blocks) );
	REQUIRE( MapHasItem(8, blocks) );

	INFO( "Block with size 8 has all the indices sorted" );
	std::vector<int> block_8 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};
	REQUIRE( blocks[8] == block_8 );

	INFO( "Block with size 4 has all the indices sorted" );
	std::vector<int> block_4 = {0, 1, 2, 3, 8, 9, 10, 11, 16, 17, 18, 19, 24, 25, 26, 27, 32, 33, 34, 35, 40, 41, 42, 43, 48, 49, 50, 51, 56, 57, 58, 59, 4, 5, 6, 7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 30, 31, 36, 37, 38, 39, 44, 45, 46, 47, 52, 53, 54, 55, 60, 61, 62, 63};
	REQUIRE( blocks[4] == block_4 );

	INFO( "Block with size 2 has all the indices sorted two and two" );
	std::vector<int> block_2 = {0, 1, 8, 9, 16, 17, 24, 25, 2, 3, 10, 11, 18, 19, 26, 27, 32, 33, 40, 41, 48, 49, 56, 57, 34, 35, 42, 43, 50, 51, 58, 59, 4, 5, 12, 13, 20, 21, 28, 29, 6, 7, 14, 15, 22, 23, 30, 31, 36, 37, 44, 45, 52, 53, 60, 61, 38, 39, 46, 47, 54, 55, 62, 63};
	REQUIRE( blocks[2] == block_2 );
}

TEST_CASE( "4x4 blocks box dimension test", "[ClusterDimension]" ) {
	int dimension = 2;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);
	int long seed = rand();

	Graph lattice = Graph(dimension, length, seed);
	std::unordered_map<int, std::vector<int>> blocks;

	// Create a cluster
	lattice.SwitchLinkBetween(0, 1);
	lattice.SwitchLinkBetween(13, 1);
	lattice.SwitchLinkBetween(13, 14);
	lattice.SwitchLinkBetween(14, 10);
	lattice.SwitchLinkBetween(6, 10);
	lattice.SwitchLinkBetween(7, 6);
	lattice.SwitchLinkBetween(7, 4);
	lattice.SwitchLinkBetween(0, 4);

	lattice.DivideGraph(blocks);

	// std::unordered_map<int, std::vector<int>> box_dimensions;
	// lattice.GetBoxDimension(blocks, box_dimensions);
}

TEST_CASE( "4x4x4 blocks are the correct size", "[ClusterDimension]" ) {
	int dimension = 3;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);
	int long seed = rand();

	Graph lattice = Graph(dimension, length, seed);
	std::unordered_map<int, std::vector<int>> blocks;

	lattice.DivideGraph(blocks);
	SECTION( "The block with system size as side length should contain all sites" )
	for (int i = 0; i < std::pow(length, dimension); ++i) {
		std::stringstream ss;
		ss << "Is site number: " << i << " in the block with side length same as system size?";
		INFO( ss.str() );

		bool has_site = std::find(blocks[length].begin(), blocks[length].end(), i) != blocks[length].end();
		REQUIRE( has_site );
	}

	SECTION( "The unsorted map blocks should contain blocks of sizes 4 and 2" )
	// Blocks should contain blocks with side length 2, 4
	REQUIRE( MapHasItem(4, blocks) );
	REQUIRE( MapHasItem(2, blocks) );

	INFO( "Block with size 4 has all the indices sorted" );
	std::vector<int> block_4 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 };
	REQUIRE( blocks[4] == block_4 );

	INFO( "Block with size 2 has all the indices sorted two and two" );
	std::vector<int> block_2 = { 0, 1, 4, 5, 16, 17, 20, 21, 32, 33, 36, 37, 48, 49, 52, 53, 8, 9, 12, 13, 24, 25, 28, 29, 2, 3, 6, 7, 18, 19, 22, 23, 40, 41, 44, 45, 56, 57, 60, 61, 34, 35, 38, 39, 50, 51, 54, 55, 10, 11, 14, 15, 26, 27, 30, 31, 42, 43, 46, 47, 58, 59, 62, 63 };
	REQUIRE( blocks[2] == block_2 );
}

TEST_CASE( "N-dimensional Site calculates correct 2D/3D neighbours", "[Site]" ) {
	int index_1 = 15;
	int length_1 = 4;
	int dimension_1 = 2;
	Site s_1 = Site(index_1, length_1, dimension_1);
	std::vector<int> correct_neighbours_1 = {12, -14, 3, -11};

	for (auto& n : s_1.neighbours) {
		auto it = std::find(correct_neighbours_1.begin(), correct_neighbours_1.end(), n.first);
		REQUIRE( it != correct_neighbours_1.end() );
	}

	int index_2 = 3;
	int length_2 = 4;
	int dimension_2 = 3;
	Site s_2 = Site(index_2, length_2, dimension_2);
	std::vector<int> correct_neighbours_2 = {-2, 0, 7, -15, 19, -51};

	for (auto& n : s_2.neighbours) {
		auto it = std::find(correct_neighbours_2.begin(), correct_neighbours_2.end(), n.first);
		REQUIRE( it != correct_neighbours_2.end() );
	}
}

TEST_CASE( "HoskenKopelman algorithm finds all 2D clusters", "[HK]" ) {
	int dimension = 2;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	int long seed = rand();
	Graph lattice = Graph(dimension, length, seed);

	// Cluster 1
	lattice.SwitchLinkBetween(0, 1);
	lattice.SwitchLinkBetween(1, 13);
	lattice.SwitchLinkBetween(13, 14);
	lattice.SwitchLinkBetween(14, 10);
	lattice.SwitchLinkBetween(10, 6);
	lattice.SwitchLinkBetween(6, 7);
	lattice.SwitchLinkBetween(7, 3);
	lattice.SwitchLinkBetween(3, 0);

	// Cluster 2
	lattice.SwitchLinkBetween(4, 5);
	lattice.SwitchLinkBetween(5, 9);
	lattice.SwitchLinkBetween(9, 8);
	lattice.SwitchLinkBetween(8, 4);

	std::unordered_map<int, std::vector<int>> clusters;
	lattice.HKIndex(clusters);

	std::unordered_map<int, std::vector<int>> correct_clusters;
	correct_clusters.insert( {1, {0, 3, 7, 6, 10, 14, 13, 1}} );
	correct_clusters.insert( {2, {4, 8, 9, 5}} );

	for (auto& value : correct_clusters) {
		REQUIRE( clusters[value.first] == value.second );
	}
}

TEST_CASE( "HoskenKopelman algorithm finds all 3D clusters", "[HK]" ) {
	int dimension = 3;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	int long seed = rand();
	Graph lattice = Graph(dimension, length, seed);

	// Cluster 1
	lattice.SwitchLinkBetween(0, 1);
	lattice.SwitchLinkBetween(5, 1);
	lattice.SwitchLinkBetween(5, 21);
	lattice.SwitchLinkBetween(25, 21);
	lattice.SwitchLinkBetween(25, 26);
	lattice.SwitchLinkBetween(30, 26);
	lattice.SwitchLinkBetween(30, 18);
	lattice.SwitchLinkBetween(19, 18);
	lattice.SwitchLinkBetween(19, 35);
	lattice.SwitchLinkBetween(34, 35);
	lattice.SwitchLinkBetween(34, 33);
	lattice.SwitchLinkBetween(32, 33);
	lattice.SwitchLinkBetween(32, 16);
	lattice.SwitchLinkBetween(0, 16);

	// Cluster 2
	lattice.SwitchLinkBetween(41, 45);
	lattice.SwitchLinkBetween(45, 61);
	lattice.SwitchLinkBetween(61, 57);
	lattice.SwitchLinkBetween(57, 41);

	// Cluster 3
	lattice.SwitchLinkBetween(42, 43);
	lattice.SwitchLinkBetween(47, 43);
	lattice.SwitchLinkBetween(46, 47);
	lattice.SwitchLinkBetween(42, 46);

	std::unordered_map<int, std::vector<int>> clusters;
	lattice.HKIndex(clusters);

	std::unordered_map<int, std::vector<int>> correct_clusters;
	correct_clusters.insert( {1, {0, 1, 5, 21, 25, 26, 30, 18, 19, 35, 34, 33, 32, 16}} );
	correct_clusters.insert( {2, {41, 45, 61, 57}} );
	correct_clusters.insert( {3, {42, 46, 47, 43}} );

	for (auto& label_and_sites : correct_clusters) {
		for (auto& site : label_and_sites.second) {
			// Every site in correct_clusters[label] should be in clusters[label]
			auto it = std::find(clusters[label_and_sites.first].begin(),
					clusters[label_and_sites.first].end(),
					site);
			REQUIRE( it != clusters[label_and_sites.first].end() );
		}
	}

	std::unordered_map<int, std::vector<int>> clusters2;
	lattice.IndexClusters(clusters2);
}

TEST_CASE( "Hosken Kopelman algorithm gives the same as brute force solution", "[HK]" ) {
	int dimension = 2;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	int long seed = rand();
	Graph lattice = Graph(dimension, length, seed);

	// Cluster 1
	lattice.SwitchLinkBetween(0, 1);
	lattice.SwitchLinkBetween(13, 1);
	lattice.SwitchLinkBetween(13, 14);
	lattice.SwitchLinkBetween(14, 10);
	lattice.SwitchLinkBetween(6, 10);
	lattice.SwitchLinkBetween(7, 6);
	lattice.SwitchLinkBetween(7, 3);
	lattice.SwitchLinkBetween(0, 3);

	// Cluster 2
	lattice.SwitchLinkBetween(4, 5);
	lattice.SwitchLinkBetween(9, 5);
	lattice.SwitchLinkBetween(9, 8);
	lattice.SwitchLinkBetween(4, 8);

	std::unordered_map<int, std::vector<int>> clusters_hk;
	lattice.HKIndex(clusters_hk);

	std::unordered_map<int, std::vector<int>> clusters_bf;
	lattice.IndexClusters(clusters_bf);

	for (auto& label_and_sites : clusters_hk) {
		for (auto& site : label_and_sites.second) {
			auto bf_solution = clusters_bf[label_and_sites.first];
			// See if site in brute force solution
			auto it = std::find(bf_solution.begin(), bf_solution.end(), site);
			REQUIRE( it != bf_solution.end() );
		}
	}
}

TEST_CASE( "GetPercolatingIndices finds all 3D percolating clusters", "[Perc]" ) {
	int dimension = 3;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	int long seed = rand();
	Graph lattice = Graph(dimension, length, seed);

	// Cluster 1 (Percolating)
	lattice.SwitchLinkBetween(0, 1);
	lattice.SwitchLinkBetween(5, 1);
	lattice.SwitchLinkBetween(5, 21);
	lattice.SwitchLinkBetween(25, 21);
	lattice.SwitchLinkBetween(25, 26);
	lattice.SwitchLinkBetween(30, 26);
	lattice.SwitchLinkBetween(30, 18);
	lattice.SwitchLinkBetween(19, 18);
	lattice.SwitchLinkBetween(19, 35);
	lattice.SwitchLinkBetween(34, 35);
	lattice.SwitchLinkBetween(34, 33);
	lattice.SwitchLinkBetween(32, 33);
	lattice.SwitchLinkBetween(32, 16);
	lattice.SwitchLinkBetween(0, 16);

	// Cluster 2
	lattice.SwitchLinkBetween(41, 45);
	lattice.SwitchLinkBetween(45, 61);
	lattice.SwitchLinkBetween(61, 57);
	lattice.SwitchLinkBetween(57, 41);

	// Cluster 3
	lattice.SwitchLinkBetween(42, 43);
	lattice.SwitchLinkBetween(47, 43);
	lattice.SwitchLinkBetween(46, 47);
	lattice.SwitchLinkBetween(42, 46);

	// Cluster 4 (Percolating)
	lattice.SwitchLinkBetween(52, 53);
	lattice.SwitchLinkBetween(54, 53);
	lattice.SwitchLinkBetween(54, 55);
	lattice.SwitchLinkBetween(52, 55);

	std::unordered_map<int, std::vector<int>> clusters;
	lattice.HKIndex(clusters);

	std::vector<int> perc_indices;
	lattice.GetPercolatingIndices(clusters, perc_indices);

	auto it1 = std::find(perc_indices.begin(), perc_indices.end(), 1);
	REQUIRE( it1 != perc_indices.end() );

	auto it4 = std::find(perc_indices.begin(), perc_indices.end(), 4);
	REQUIRE( it4 != perc_indices.end() );
}

TEST_CASE( "GetPercolatingIndices finds all 2D percolating clusters", "[Perc]" ) {
	int dimension = 2;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	int long seed = rand();
	Graph lattice = Graph(dimension, length, seed);

	// Cluster 1 (Percolating)
	lattice.SwitchLinkBetween(0, 1);
	lattice.SwitchLinkBetween(1, 13);
	lattice.SwitchLinkBetween(13, 14);
	lattice.SwitchLinkBetween(10, 14);
	lattice.SwitchLinkBetween(10, 6);
	lattice.SwitchLinkBetween(7, 6);
	lattice.SwitchLinkBetween(7, 3);
	lattice.SwitchLinkBetween(0, 3);

	// Cluster 2
	lattice.SwitchLinkBetween(4, 5);
	lattice.SwitchLinkBetween(9, 5);
	lattice.SwitchLinkBetween(9, 8);
	lattice.SwitchLinkBetween(4, 8);

	std::unordered_map<int, std::vector<int>> clusters;
	lattice.HKIndex(clusters);

	std::vector<int> perc_indices;
	lattice.GetPercolatingIndices(clusters, perc_indices);

	auto it1 = std::find(perc_indices.begin(), perc_indices.end(), 1);
	REQUIRE( it1 != perc_indices.end() );
}

TEST_CASE( "GetSign works for 2D graph with L=4", "[GetSign]" ) {
	int dimension = 2;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	int long seed = rand();
	Graph lattice = Graph(dimension, length, seed);

	std::unordered_map<int, int> positive_links = {{ 0, 1 }, {1, 2}, {2, 3}, {3, 0}, {4, 5}, {5, 6}, {6, 7}, {7, 4}, {8, 9}, {9, 10}, {11, 8}, {12, 13}, {13, 14}, {14, 15}, {15, 12},
		{0, 4}, {1, 5}, {2, 6}, {3, 7}, {4, 8}, {5, 9}, {6, 10}, {7, 11}, {8, 12}, {9, 13}, {10, 14}, {11, 15}, {12, 0}, {13, 1}, {14, 2}, {15, 3}};

	for (auto& link : positive_links) {
		REQUIRE( lattice.GetSign(link.first, link.second) == +1 );
		REQUIRE( lattice.GetSign(link.second, link.first) == -1 );
	}
}

TEST_CASE( "SaveGraphToFile and LoadGraphFromFile gives the same graph", "[GraphToFile]" ) {
	int dimension = 2;
	int length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	int long seed = rand();
	Graph lattice = Graph(dimension, length, seed);

	lattice.SwitchLinkBetween(0, 1);
	lattice.SwitchLinkBetween(1, 5);
	lattice.SwitchLinkBetween(5, 6);
	lattice.SwitchLinkBetween(6, 10);
	lattice.SwitchLinkBetween(10, 9);
	lattice.SwitchLinkBetween(9, 5);
	lattice.SwitchLinkBetween(5, 6);
	lattice.SwitchLinkBetween(6, 7);
	lattice.SwitchLinkBetween(7, 3);
	lattice.SwitchLinkBetween(3, 0);

	std::string filename = "test_graph.txt";
	SaveGraphToFile(filename, lattice);

	Graph compare_lattice = Graph(dimension, length, seed);
	LoadGraphFromFile(filename, compare_lattice);

	for (int i = 0; i < std::pow(length, dimension); ++i) {
		std::vector<int> neighbours;
		lattice.GetLinkedNeighbours(i, neighbours);

		std::vector<int> compare_ns;
		lattice.GetLinkedNeighbours(i, compare_ns);

		REQUIRE( neighbours.size() == compare_ns.size() );
		for (auto& n : neighbours) {
			REQUIRE( VecHasItem(compare_ns, n) );
		}
	}
}
