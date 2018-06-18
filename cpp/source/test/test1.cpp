#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file
#include "catch.hpp"
#include "cmath"
#include "Graph.h"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

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
			std::cout << n << "\n";
		}
	}
	// REQUIRE( 4 == 5 );
}
