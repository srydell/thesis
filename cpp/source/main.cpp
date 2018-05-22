#include <iostream>
#include <vector>
#include <cmath>
#include "Site.h"

int main(){
	// Length of one side of the lattice
	int length = 800;
	// 2D, 3D, ...
	int dimension = 2;
	// Initialize the vector with a reserve of length^dimension
	std::vector<Site> gitter;
	gitter.reserve(std::pow(length, dimension));

	for (int index = 0; index < std::pow(length,dimension); ++index) {
		gitter.push_back(Site(index, length));
	}
	std::cout << gitter.size() << "\n";
}
