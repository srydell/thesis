#include "Graph.h"
#include <iostream>
#include <string>

int main(){
	// Length of one side of the lattice
	int length = 4;
	// 2D, 3D, ...
	const int dimension = 2;
	Graph g(dimension, length);

	// g.PrintGraph();
	// Simulation
	try {
		std::vector<unsigned> neighbours;

		std::cout << "Switching links between 6 - 10, 10 - 9, 9 - 8, and 8 - 12" << "\n\n";
		g.SwitchLinkBetween(6, 10);
		g.SwitchLinkBetween(10, 9);
		g.SwitchLinkBetween(9, 8);
		g.SwitchLinkBetween(8, 12);

		// g.GetLinkedNeighbours(9, neighbours);

		std::unordered_map<unsigned, std::vector<unsigned>> clusters;
		g.IndexClusters(clusters);

		// std::cout << "Switch 9 and 13" << "\n";
		// g.SwitchLinkBetween(9, 13);
		// neighbours = g.GetLinkedNeighbours(9);

	} catch(std::string error) {
		std::cout << error << std::endl;
	}
}
