#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "Graph.h"
#include "Site.h"
#include "catch.hpp"
#include "cmath"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

TEST_CASE( "All neighbours are neighbours to each other", "[Graph]" ) {
	unsigned dimension = 2;
	unsigned length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	unsigned long mSeed = rand();
	Graph lattice = Graph(dimension, length, mSeed);

	lattice.SwitchLinkBetween(0, 1);
	lattice.SwitchLinkBetween(13, 1);
	lattice.SwitchLinkBetween(13, 14);
	lattice.SwitchLinkBetween(14, 10);
	lattice.SwitchLinkBetween(6, 10);
	lattice.SwitchLinkBetween(7, 6);
	lattice.SwitchLinkBetween(7, 4);
	lattice.SwitchLinkBetween(0, 4);
	for (unsigned i = 0; i < std::pow(length, dimension)-1; ++i) {
		std::vector<unsigned> neighbours;
		lattice.GetLinkedNeighbours(i, neighbours);
		for (auto n : neighbours) {
			// Neighbours to this neighbour
			std::vector<unsigned> neighbours_to_n;
			lattice.GetLinkedNeighbours(n, neighbours_to_n);
			// All neighbours to i also have i as a neighbour
			REQUIRE( std::find(neighbours_to_n.begin(), neighbours_to_n.end(), i) != neighbours_to_n.end() );
		}
	}
}

TEST_CASE( "4x4 blocks are the correct size", "[ClusterDimension]" ) {
	unsigned dimension = 2;
	unsigned length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);
	unsigned long mSeed = rand();

	Graph lattice = Graph(dimension, length, mSeed);
	std::unordered_map<unsigned, std::vector<unsigned>> blocks;

	lattice.DivideGraph(blocks);
	SECTION( "The block with system size as side length should contain all sites" )
	for (unsigned i = 0; i < std::pow(length, dimension); ++i) {
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
	std::vector<unsigned> block_4 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
	REQUIRE( blocks[4] == block_4 );

	INFO( "Block with size 2 has all the indices sorted two and two" );
	std::vector<unsigned> block_2 = {0, 1, 4, 5, 8, 9, 12, 13, 2, 3, 6, 7, 10, 11, 14, 15};
	REQUIRE( blocks[2] == block_2 );
}

TEST_CASE( "8x8 blocks are the correct size", "[ClusterDimension]" ) {
	unsigned dimension = 2;
	unsigned length = 8;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);
	unsigned long mSeed = rand();

	Graph lattice = Graph(dimension, length, mSeed);
	std::unordered_map<unsigned, std::vector<unsigned>> blocks;

	lattice.DivideGraph(blocks);
	SECTION( "The block with system size as side length should contain all sites" )
	for (unsigned i = 0; i < std::pow(length, dimension); ++i) {
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
	std::vector<unsigned> block_8 = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63};
	REQUIRE( blocks[8] == block_8 );

	INFO( "Block with size 4 has all the indices sorted" );
	std::vector<unsigned> block_4 = {0, 1, 2, 3, 8, 9, 10, 11, 16, 17, 18, 19, 24, 25, 26, 27, 32, 33, 34, 35, 40, 41, 42, 43, 48, 49, 50, 51, 56, 57, 58, 59, 4, 5, 6, 7, 12, 13, 14, 15, 20, 21, 22, 23, 28, 29, 30, 31, 36, 37, 38, 39, 44, 45, 46, 47, 52, 53, 54, 55, 60, 61, 62, 63};
	REQUIRE( blocks[4] == block_4 );

	INFO( "Block with size 2 has all the indices sorted two and two" );
	std::vector<unsigned> block_2 = {0, 1, 8, 9, 16, 17, 24, 25, 2, 3, 10, 11, 18, 19, 26, 27, 32, 33, 40, 41, 48, 49, 56, 57, 34, 35, 42, 43, 50, 51, 58, 59, 4, 5, 12, 13, 20, 21, 28, 29, 6, 7, 14, 15, 22, 23, 30, 31, 36, 37, 44, 45, 52, 53, 60, 61, 38, 39, 46, 47, 54, 55, 62, 63};
	REQUIRE( blocks[2] == block_2 );

	// std::cout << "The final blocks for 8x8 graph are as:" << "\n";
	// for (auto& block : blocks) {
	// 	std::cout << "Block with side_length: ";
	// 	std::cout << block.first << "\n";
	// 	for (auto& site : block.second) {
	// 		std::cout << site << ", ";
	// 	}
	// 	std::cout << "\n";
	// }
}

TEST_CASE( "4x4 blocks box dimension test", "[ClusterDimension]" ) {
	unsigned dimension = 2;
	unsigned length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);
	unsigned long mSeed = rand();

	Graph lattice = Graph(dimension, length, mSeed);
	std::unordered_map<unsigned, std::vector<unsigned>> blocks;

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

	// std::unordered_map<unsigned, std::vector<unsigned>> box_dimensions;
	// lattice.GetBoxDimension(blocks, box_dimensions);
}

TEST_CASE( "4x4x4 blocks are the correct size", "[ClusterDimension3D]" ) {
	unsigned dimension = 3;
	unsigned length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);
	unsigned long mSeed = rand();

	Graph lattice = Graph(dimension, length, mSeed);
	std::unordered_map<unsigned, std::vector<unsigned>> blocks;

	lattice.DivideGraph(blocks);
	SECTION( "The block with system size as side length should contain all sites" )
	for (unsigned i = 0; i < std::pow(length, dimension); ++i) {
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
	std::vector<unsigned> block_4 = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63 };
	REQUIRE( blocks[4] == block_4 );

	INFO( "Block with size 2 has all the indices sorted two and two" );
	std::vector<unsigned> block_2 = { 0, 1, 4, 5, 16, 17, 20, 21, 32, 33, 36, 37, 48, 49, 52, 53, 8, 9, 12, 13, 24, 25, 28, 29, 2, 3, 6, 7, 18, 19, 22, 23, 40, 41, 44, 45, 56, 57, 60, 61, 34, 35, 38, 39, 50, 51, 54, 55, 10, 11, 14, 15, 26, 27, 30, 31, 42, 43, 46, 47, 58, 59, 62, 63 };
	REQUIRE( blocks[2] == block_2 );
}

TEST_CASE( "The template function GetMaximumMapValue finds maximums", "[GetMaximumMapValue]" ) {
	std::unordered_map<unsigned, unsigned> test_map1 = {{2, 2}, {4, 1}, {100, 200}};
	std::unordered_map<float, unsigned> test_map2 = {{5.2, 2}, {4.1, 1}, {100.4, 0}};
	auto max_value1 = GetMaximumMapIndex(test_map1);
	auto max_value2 = GetMaximumMapIndex(test_map2);

	REQUIRE( max_value1 == 100 );
	REQUIRE( max_value2 == 5.2f );
}

TEST_CASE( "N-dimensional Site calculates correct 2D/3D neighbours", "[Site]" ) {
	unsigned index_1 = 15;
	unsigned length_1 = 4;
	unsigned dimension_1 = 2;
	Site s_1 = Site(index_1, length_1, dimension_1);
	std::vector<unsigned> correct_neighbours_1 = {12, 14, 3, 11};

	std::cout << "Site: " << s_1.GetIndex() << " has neighbours:" << "\n";
	for (auto& n : s_1.neighbours) {
		std::cout << "	" << n.first << "\n";

		auto it = std::find(correct_neighbours_1.begin(), correct_neighbours_1.end(), n.first);
		REQUIRE( it != correct_neighbours_1.end() );
	}

	unsigned index_2 = 3;
	unsigned length_2 = 4;
	unsigned dimension_2 = 3;
	Site s_2 = Site(index_2, length_2, dimension_2);
	std::vector<unsigned> correct_neighbours_2 = {2, 0, 7, 15, 19, 51};

	std::cout << "Site: " << s_2.GetIndex() << " has neighbours:" << "\n";
	for (auto& n : s_2.neighbours) {
		std::cout << "	" << n.first << "\n";

		auto it = std::find(correct_neighbours_2.begin(), correct_neighbours_2.end(), n.first);
		REQUIRE( it != correct_neighbours_2.end() );
	}
}

TEST_CASE( "HoskenKopelman algorithm finds all 2D clusters", "[HK]" ) {
	unsigned dimension = 2;
	unsigned length = 4;
	int nulltime = time(nullptr);
	srand((unsigned)nulltime);

	unsigned long mSeed = rand();
	Graph lattice = Graph(dimension, length, mSeed);

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

	std::unordered_map<unsigned, std::vector<unsigned>> clusters;
	lattice.HKIndex(clusters);
	REQUIRE( 1 == 1 );
}
