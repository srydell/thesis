#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "Graph.h"
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
	REQUIRE( HasItem(4, blocks) );
	REQUIRE( HasItem(2, blocks) );

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
	REQUIRE( HasItem(2, blocks) );
	REQUIRE( HasItem(4, blocks) );
	REQUIRE( HasItem(8, blocks) );

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

	std::vector<double> box_dimensions;
	lattice.GetBoxDimension(blocks, box_dimensions);
}

TEST_CASE( "The template function GetMaximumMapValue finds maximums", "[GetMaximumMapValue]" ) {
	std::unordered_map<unsigned, unsigned> test_map1 = {{2, 2}, {4, 1}, {100, 200}};
	std::unordered_map<float, unsigned> test_map2 = {{5.2, 2}, {4.1, 1}, {100.4, 0}};
	auto max_value1 = GetMaximumMapIndex(test_map1);
	auto max_value2 = GetMaximumMapIndex(test_map2);

	REQUIRE( max_value1 == 100 );
	REQUIRE( max_value2 == 5.2f );
}
