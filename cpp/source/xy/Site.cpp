#include "Site.h"
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

Site::Site(int index, int length, int dimension) {
	// The index of this site
	this->mRootIndex = index;
	// Populate the neighbours unordered_map
	AddNeighbours(length, dimension);
}

/**
* @brief: Create the neighbours to site according to periodic boundary conditions
*
* @param: int length
*       : int dimension
*
* @return: void
*/
void Site::AddNeighbours(int length, int dimension) {
	// --- 2D ---
	// x + y * L = N
	//
	// x = N - y * L
	// y = N // L
	//
	// --- General ---
	// x + y * L + z * L^2 + ... = N
	int startingWeight = 0;

	// Find the (x, y, z, ...) vector corresponding to the given index (mRootIndex)
	std::vector<int> xyz;
	xyz.reserve(dimension);
	auto temp_index = mRootIndex;
	for (int i = 0; i < dimension; ++i) {

		// std::cout << "Xyz index: " << i << "\n";
		// std::cout << "Adding: temp_index % length : " << temp_index << " % " << length << " : " << temp_index % length << "\n";

		xyz.push_back(temp_index % length);
		temp_index = std::floor(temp_index / length);
	}

	// std::cout << "On site with index: " << mRootIndex << " with dimension: " << dimension << " and length: " << length << "\n";
	// std::cout << "Found xyz: (";
	// for (auto& x : xyz) {
	// 	std::cout << x << ", ";
	// }
	// std::cout << ")" << "\n";

	// Go through and check all directions, adding them to neighbours
	for (auto& x_i : xyz) {
		if ((x_i + 1) < length) {
			// + direction is within the graph
			x_i++;

			// std::cout << "x_i on the + border. Adding: " << CalcIndexFromVector(xyz, length) << "\n";

			neighbours.insert({CalcIndexFromVector(xyz, length), startingWeight});
			// Make sure xyz is returned to its original state
			x_i--;
		} else {
			// + direction was on border - Apply periodic boundary conditions
			// x -> 0
			auto x_old = x_i;
			x_i = 0;

			// std::cout << "x_i not on the + border. Adding: " << CalcIndexFromVector(xyz, length) << "\n";

			neighbours.insert({CalcIndexFromVector(xyz, length), startingWeight});
			// Make sure xyz is returned to its original state
			x_i = x_old;
		}

		if (x_i != 0) {
			// - direction is within the graph
			x_i--;

			// std::cout << "x_i on the - border. Adding: " << -1 * CalcIndexFromVector(xyz, length) << "\n";

			neighbours.insert({-1 * CalcIndexFromVector(xyz, length), startingWeight});
			// Make sure xyz is returned to its original state
			x_i++;
		} else {
			// - direction was on border - Apply periodic boundary conditions
			// x -> length - 1
			auto x_old = x_i;
			x_i = length - 1;

			// std::cout << "x_i not on the - border. Adding: " << -1 * CalcIndexFromVector(xyz, length) << "\n";

			neighbours.insert({-1 * CalcIndexFromVector(xyz, length), startingWeight});
			// Make sure xyz is returned to its original state
			x_i = x_old;
		}
	}
}

/**
* @brief: Calculate the index corresponding to the values in xyz = (x_0, x_1, ..., x_N)
*
* @param: std::vector<int>& xyz
*
* @return: int
*/
int Site::CalcIndexFromVector(std::vector<int>& xyz, int length) {
	// index = x_0 + L * x_1 + L^2 * x_2 + ...
	auto index = 0;
	auto exponent = 0;
	for (auto& x_i : xyz) {
		// x_i * L^i
		index += x_i * std::pow(length, exponent);
		exponent++;
	}
	return index;
}

/**
* @brief: Return the index of the site
*
* @param: 
*
* @return: int
*/
int Site::GetIndex() {
	return mRootIndex;
}
