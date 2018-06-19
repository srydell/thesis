#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "cmath"
#include "Graph.h"
#include <algorithm>

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
