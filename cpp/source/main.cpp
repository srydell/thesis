#include "Graph.h"
#include <iostream>
#include <string>

int main(){
	// Length of one side of the lattice
	int length = 4;
	// 2D, 3D, ...
	const int dimension = 2;
	Graph g(dimension, length);

	// Simulation
	try {

		g.SwitchLinkBetween(9, 10);
		g.SwitchLinkBetween(9, 13);
		std::vector<unsigned> neighbours;
		neighbours = g.GetLinkedNeighbours(9);
		for (unsigned test : neighbours) {
			std::cout << test << "\n";
		}

		g.SwitchLinkBetween(9, 13);
		neighbours = g.GetLinkedNeighbours(9);
		for (unsigned test : neighbours) {
			std::cout << test << "\n";
		}

	} catch(std::string error) {
		std::cout << error << std::endl;
	}
}
