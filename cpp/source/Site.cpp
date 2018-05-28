#include <unordered_map>
#include <cmath>
#include <iostream>

class Site
{
public:
	// Constructor
	Site (unsigned index, unsigned length);

	// Fields
	std::unordered_map<unsigned, bool> neighbours;
	unsigned GetIndex();
private:
	unsigned mRootIndex;

	// Functions
	void AddNeighbours(unsigned mRootIndex, unsigned length);
};

// Constructor of Site
Site::Site(unsigned index, unsigned length) {
	// The index of this site
	mRootIndex = index;
	// Populate the neighbours unordered_map
	AddNeighbours(mRootIndex, length);
}

/**
* @brief: Create the neighbours to site according to periodic boundary conditions
*
* @param: unsigned mRootIndex
*       : unsigned length
*
* @return: void
*/
void Site::AddNeighbours(unsigned mRootIndex, unsigned length) {
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
	// Assume 2D
	// Get the indices
	// Important that the division is a floor (//) here.
	// Since both mRootIndex and length are unsigned / is a floor operation
	unsigned y = mRootIndex/length;
	unsigned x = mRootIndex - length*y;
	bool startingWeight = 0;

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