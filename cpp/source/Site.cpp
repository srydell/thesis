#include "Site.h"
#include <cmath>
#include <iostream>
#include <unordered_map>

// Constructor of Site
Site::Site(unsigned index, unsigned length) {
	// The index of this site
	mRootIndex = index;
	// Populate the neighbours unordered_map
	AddNeighbours(length);
}

/**
* @brief: Create the neighbours to site according to periodic boundary conditions
*
* @param: unsigned length
*
* @return: void
*/
void Site::AddNeighbours(unsigned length) {
	// --- 3D ---
	// x + y * L + z * L * L = N
	//
	// x = N - y * L - z * L * L
	// y = N // L - z * L
	// z = N // (L * L)
	//
	// --- 2D ---
	// x + y * L = N
	//
	// x = N - y * L
	// y = N // L
	//
	// --- General ---
	// x + y * L + z * L^2 + ... = N
	//
	// let k be some index in the graph
	// x = k % L
	// y = floor(k, L) % L
	// z = floor(k, L^2) % L
	// ...
	//
	// Implementation of general:
	// vector<unsigned> convert(index) {
	// 	vector<unsigned> xyz;
	// 	xyz.reserve(mDimension);
	// 	for (unsigned i = 0; i < mDimension; ++i) {
	// 		xyz[i] = index % mLength;
	// 		index = floor(index / mLength);
	// 	}
	// 	return xyz
	// }
	//
	// Assume 2D
	// Get the indices
	// Important that the division is a floor (//) here.
	// Since both mRootIndex and length are unsigned / is a floor operation
	unsigned y = mRootIndex / length;
	unsigned x = mRootIndex - length*y;
	bool startingWeight = 0;

	// TODO: Rewrite this for arbitrary dimension. It should probably take the dimension as a parameter then
	// Find the neighbours with periodic boundary conditions
	// Check x + 1
	if ((x+1)<length){
		neighbours.insert({x+1 + length*y, startingWeight});
	} else {
		// x is on the 'top' border, so set it to zero
		neighbours.insert({0 + length*y, startingWeight});
	}
	// Check x - 1 < 0
	if (x != 0){
		neighbours.insert({x-1 + length*y, startingWeight});
	} else {
		// x is on the 'bottom' border, so set it to length-1
		neighbours.insert({length-1 + length*y, startingWeight});
	}

	// Check y + 1
	if ((y+1)<length){
		neighbours.insert({x + length*(y+1), startingWeight});
	} else {
		// y is on the 'top' border, so set it to zero
		neighbours.insert({x + 0, startingWeight});
	}
	// Check y - 1 < 0
	if (y != 0){
		neighbours.insert({x + length*(y-1), startingWeight});
	} else {
		// y is on the 'bottom' border, so set it to length-1
		neighbours.insert({x + length*(length-1), startingWeight});
	}

	// std::cout << "x is : " << x << "\n";
	// std::cout << "y is : " << y << "\n";
	// std::cout << "\n";
	// for (std::pair<unsigned, bool> element : neighbours) {
	// 	std::cout << element.first << " : " << element.second << std::endl;
	// }
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
