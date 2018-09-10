#include "catch.hpp"
#include "cmath"
#include "utils.h"
#include <algorithm>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

TEST_CASE( "The template function GetMaximumMapValue finds maximums", "[GetMaximumMapValue]" ) {
	std::unordered_map<int, int> test_map1 = {{2, 2}, {4, 1}, {100, 200}};
	std::unordered_map<float, int> test_map2 = {{5.2, 2}, {4.1, 1}, {100.4, 0}};
	auto max_value1 = GetMaximumMapIndex(test_map1);
	auto max_value2 = GetMaximumMapIndex(test_map2);

	REQUIRE( max_value1 == 100 );
	REQUIRE( max_value2 == 5.2f );
}
