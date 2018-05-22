#include <iostream>
#include <set>
#include <string>
#include <vector>
#include <unordered_map>
#include <cmath>

class Site
{
public:
	// Constructor/destructor
	Site (int index, int length);

	// Fields
	std::unordered_map<int, bool> neighbours;
private:
	int rootIndex;

	// Functions
	void addNeighbours(int rootIndex, int length, std::unordered_map<int, bool> neighbours);
};

// constructor of Site
Site::Site(int index, int length)
{
	// The index of this site
	rootIndex = index;
	addNeighbours(rootIndex, length, neighbours);
}

void Site::addNeighbours(int rootIndex, int length, std::unordered_map<int, bool> neighbours) {
	// --- 3D ---
	// x + y * L + z * L^2 = N
	// x = N - y * L - z * L^2
	// y = N // L - z * L
	// z = N // L^2
	//
	// --- 2D ---
	// x + y * L = N
	// x = N - y * L
	// y = N // L
	//
	// Assume 2D
	// Get the indices
	// Important that the division is a floor (//) here.
	// Since both rootIndex and length are ints / is a floor operation
	int y = rootIndex/length;
	int x = rootIndex - length*y;
	bool startingWeight = 0;

	// Find the neighbours with periodic boundary conditions
	// Check x + 1
	if ((x+1)<length){
		neighbours.insert({x+1 + length*y, startingWeight});
	} else {
		// x is on the 'top' border, so set it to zero
		neighbours.insert({0 + length*y, startingWeight});
	}
	// Check x - 1
	if ((x-1)>=0){
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
	// Check y - 1
	if ((y-1)>=0){
		neighbours.insert({x + length*(y-1), startingWeight});
	} else {
		// y is on the 'bottom' border, so set it to length-1
		neighbours.insert({x + length*(length-1), startingWeight});
	}

	// std::cout << "x is : " << x << "\n";
	// std::cout << "y is : " << y << "\n";
	// std::cout << "\n";
	// for (std::pair<int, bool> element : neighbours) {
	// 	std::cout << element.first << " : " << element.second << std::endl;
	// }

}

int main(){
	// Length of one side of the lattice
	int length = 800;
	// 2D, 3D, ...
	int dimension = 2;
	// Initialize the vector with a reserve of length^dimension
	std::vector<Site> gitter;
	// gitter.reserve(length^dimension);

	// for (int i = 0; i < 16; ++i) {
	// 	std::cout << "index is : " << i << "\n";
	// 	Site test(i, length);
	// }

	for (int index = 0; index < std::pow(length,dimension); ++index) {
		gitter.push_back(Site(index, length));
	}
	std::cout << gitter.size() << "\n";
}
