#include "Graph.h"
#include <iostream>
#include <string>

int main(){
	// Length of one side of the lattice
	int length = 4;
	// 2D, 3D, ...
	const int dimension = 2;
	Graph g(dimension, length);

	try {
		g.switchLinkBetween(9, 10);
		g.switchLinkBetween(9, 10);
	} catch(std::string error) {
		std::cout << error << std::endl;
	}
}
