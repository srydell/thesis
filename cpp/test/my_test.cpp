#define CATCH_CONFIG_MAIN
#include "catch.hpp"
#include "Graph.h"
#include "IsingWorm.h"

TEST_CASE("UpdateCorrelationFunction", "[testing]") {
	Graph lattice = Graph(4, 4);
	REQUIRE( 2 == 2 );
}

TEST_CASE("simple test case", "[testing]") {
	REQUIRE( 2 == 2 );
}

TEST_CASE("Ajkldsf test case", "[testing]") {
	REQUIRE( 5 == 5 );
}

TEST_CASE("Another test case", "[testing]") {
	REQUIRE( 4 == 4 );
}
