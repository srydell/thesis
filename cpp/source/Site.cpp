#include "Site.h"
#include <cmath>
#include <iostream>
#include <vector>
#include <unordered_map>

// Constructor of Site
Site::Site(unsigned index, unsigned length, unsigned dimension) {
	// The index of this site
	this->mRootIndex = index;
	// Populate the neighbours unordered_map
	AddNeighbours(length, dimension);
}

/**
* @brief: Create the neighbours to site according to periodic boundary conditions
*
* @param: unsigned length
*       : unsigned dimension
*
* @return: void
*/
void Site::AddNeighbours(unsigned length, unsigned dimension) {
	// --- 2D ---
	// x + y * L = N
	//
	// x = N - y * L
	// y = N // L
	//
	// --- General ---
	// x + y * L + z * L^2 + ... = N
	bool startingWeight = 0;

	// Find the (x, y, z, ...) vector corresponding to the given index (mRootIndex)
	std::vector<unsigned> xyz;
	xyz.reserve(dimension);
	auto temp_index = mRootIndex;
	for (unsigned i = 0; i < dimension; ++i) {

		// std::cout << "Xyz index: " << i << "\n";
		// std::cout << "Adding: temp_index % length : " << temp_index << " % " << length << " : " << temp_index % length << "\n";

		xyz.push_back(temp_index % length);
		temp_index = std::floor(temp_index / length);
	}

	// std::vector<unsigned> xyz;
	// xyz.reserve(mDimension);
	// for (unsigned i = 0; i < mDimension; ++i) {
	// 	xyz[i] = index % mLength;
	// 	index = std::floor(index / mLength);
	// }

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

			// std::cout << "x_i on the - border. Adding: " << CalcIndexFromVector(xyz, length) << "\n";

			neighbours.insert({CalcIndexFromVector(xyz, length), startingWeight});
			// Make sure xyz is returned to its original state
			x_i++;
		} else {
			// - direction was on border - Apply periodic boundary conditions
			// x -> length - 1
			auto x_old = x_i;
			x_i = length - 1;

			// std::cout << "x_i not on the - border. Adding: " << CalcIndexFromVector(xyz, length) << "\n";

			neighbours.insert({CalcIndexFromVector(xyz, length), startingWeight});
			// Make sure xyz is returned to its original state
			x_i = x_old;
		}
	}

	// std::cout << "x is : " << x << "\n";
	// std::cout << "y is : " << y << "\n";
	// std::cout << "\n";
	// for (std::pair<unsigned, bool> element : neighbours) {
	// 	std::cout << element.first << " : " << element.second << std::endl;
	// }
}

/**
* @brief: Calculate the index corresponding to the values in xyz = (x_0, x_1, ..., x_N)
*
* @param: std::vector<unsigned>& xyz
*
* @return: unsigned
*/
unsigned Site::CalcIndexFromVector(std::vector<unsigned>& xyz, unsigned length) {
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
* @return: unsigned
*/
unsigned Site::GetIndex() {
	return mRootIndex;
}
