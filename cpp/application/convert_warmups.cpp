#include "Graph.h"
#include "utils.h"
#include "xyHelper.h"
#include "xyWorm.h"
#include <cmath>
#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

int main(int argc, char** argv) {
	try {
		// std::ios_base::sync_with_stdio(false);

		std::string input_file = "";
		// Retrieve the (non-option) argument:
		if ( (argc <= 1) || (argv[argc-1] == NULL) || (argv[argc-1][0] == '-') ) {  // there is NO input_file...
			std::cerr << "No argument provided! Exiting..." << "\n";
			return 1;
		}
		else {  // there is an input...
			input_file = argv[argc-1];
		}

		if (FileExists(input_file)) {
			std::cout << "Input is a file!" << "\n";

			double T = 0;
			double length = 0;
			{
				std::regex pattern {R"(xyl(\d+)t(\d*\.?\d*).txt)"};
				std::smatch matches;
				if (std::regex_search(input_file, matches, pattern)) {
					std::stringstream(matches[1].str()) >> length;
					std::stringstream(matches[2].str()) >> T;

					std::cout << "Input match file found: " << matches[0].str() << "\n";
					std::cout << "Length is: " << length << "\n";
					std::cout << "Temperature is: " << T << "\n";
				} else {
					std::cerr << "No Length or temperature could be found. Exiting..." << "\n";
					return 1;
				}
			}

			const int dimension = 3;

			int nulltime = time(nullptr);
			srand((unsigned)nulltime + getpid());

			// Create a new graph
			int long seed = rand();
			Graph lattice(dimension, length, seed + getpid());

			// Bond strength J = 1
			double K = 1/T;

			auto previous_data = LoadGraphFromFile(input_file, lattice);
			std::cout << "Previous energy: " << previous_data.energy << "\n";
			std::cout << "Previous winding number: " << previous_data.winding_number << "\n";

			// E = K/2 * sum(J^2)
			double energy = 0;
			// w = 1/L * sum(J_x)
			double winding_number = 0;

			// Calculate energy/winding_number
			for (int i = 0; i < std::pow(length, dimension); ++i) {
				std::vector<int> neighbours;
				lattice.GetLinkedNeighbours(i, neighbours);
				for (auto& n : neighbours) {

					// Special case for 0. Take all neighbours
					if (i == 0) {
						auto link = lattice.GetLink(i, std::abs(n));
						energy += std::pow(link, 2);
						// NOTE: link is the physical link
						// so SignIfInXDirection is only needed to be zero if not in x direction
						// Therefore the abs(...)
						winding_number += std::abs(SignIfInXDirection(i, n, length)) * link;
					} else {
						// Avoid double counting
						if (n > 0) {
							auto link = lattice.GetLink(i, n);
							energy += std::pow(link, 2);
							// NOTE: link is the physical link
							// so SignIfInXDirection is only needed to be zero if not in x direction
							// Therefore the abs(...)
							winding_number += std::abs(SignIfInXDirection(i, n, length)) * link;
						}
					}
				}
			}

			energy = (K / 2) * energy;
			winding_number = (1 / length) * winding_number;
			std::cout << "Energy found: " << energy << "\n";
			std::cout << "Winding number found: " << winding_number << "\n";

			// std::stringstream ss;
			// ss << input_file << "_mod";
			// SaveGraphToFile(ss.str(), lattice, {winding_number, energy, 0.0});
			SaveGraphToFile(input_file, lattice, {winding_number, energy, 0.0});
		} else {
			std::cerr << "Input is not a file. Exiting..." << "\n";
		}
	} catch(std::string& error) {
		std::cout << error << "\n";
	}
}
