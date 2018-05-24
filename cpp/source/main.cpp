#include "Graph.h"
#include <iostream>
#include <random>
#include <string>

int main(){
	// Type of random number distribution
	std::uniform_real_distribution<double> dist(0.0, 1.0);  //(min, max)

	//Mersenne Twister: Good quality random number generator
	std::mt19937 rng; 
	//Initialize with non-deterministic seeds
	rng.seed(std::random_device{}());

    // std::cout << dist(rng) << std::endl;

	// Length of one side of the lattice
	int length = 4;
	// 2D, 3D, ...
	const int dimension = 2;
	Graph g(dimension, length);

	// Simulation
	try {
		std::vector<unsigned> neighbours;

		std::cout << "Switch 9 and 10" << "\n";
		g.SwitchLinkBetween(9, 10);
		std::cout << "Switch 9 and 13" << "\n";
		g.SwitchLinkBetween(9, 13);
		neighbours = g.GetLinkedNeighbours(9);

		std::cout << "Switch 9 and 13" << "\n";
		g.SwitchLinkBetween(9, 13);
		neighbours = g.GetLinkedNeighbours(9);

		// for (unsigned test : neighbours) {
		// 	std::cout << test << "\n";
		// }

		unsigned rNeighbour = g.GetRandomNeighbour(9, 10, dist(rng));
		std::cout << rNeighbour << "\n";
		unsigned notNeighbour = g.GetRandomNeighbour(9, 11, dist(rng));
		std::cout << notNeighbour << "\n";

	} catch(std::string error) {
		std::cout << error << std::endl;
	}
}
