#define CATCH_CONFIG_MAIN
#include "catch.hpp"

TEST_CASE("A test case", "[testing]") {
	REQUIRE( 1 == 1 );
}

TEST_CASE("simple test case", "[testing]") {
	REQUIRE( 2 == 2 );
}

TEST_CASE("teh simplest test case", "[testing]") {
	REQUIRE( 4 == 3 );
}

TEST_CASE("Ajkldsf test case", "[testing]") {
	REQUIRE( 5 == 5 );
}

TEST_CASE("Another test case", "[testing]") {
	REQUIRE( 4 == 4 );
}
