#include "Graph.h"
#include "utils.h"
#include <cmath>
#include <iostream>
#include <unordered_map>
#include <vector>

/**
* @brief: Divide the mGraph into blocks recursively for fractal dimension analysis
*         by calling DivideGraphRec with correct input
*
* @param: std::unordered_map<unsigned, std::vector<unsigned>>& blocks
*
* @return: std::unordered_map<unsigned, std::vector<unsigned>>
*/
void Graph::DivideGraph(std::unordered_map<unsigned, std::vector<unsigned>>& blocks){
	// blocks will be divided but each side_length will still have all the sites
	// (only in a different order where every side_length^mDimension number of sites is one block)
	unsigned l = mLength;
	while (l >= 2) {
		blocks[l].reserve(std::pow(mLength, mDimension));
		l = l/2;
	}
	DivideGraphRec(blocks, mLength, 0, mLength);
}

/**
* @brief: Divide the mGraph into blocks recursively for fractal dimension analysis
*
* @param: std::unordered_map<unsigned, std::vector<unsigned>>& blocks
*         unsigned side_length
*         unsigned start
*         unsigned system_size
*
* @return: void
*/
void Graph::DivideGraphRec(std::unordered_map<unsigned, std::vector<unsigned>>& blocks, unsigned side_length, unsigned start, unsigned system_size){
	// TODO: Test this for 3D graph

	std::cout << "\n";
	std::cout << "Call to DivideGraphRec with parameters:" << "\n";
	std::cout << "side_length: " << side_length << "\n";
	std::cout << "start: " << start << "\n";
	std::cout << "system_size: " << system_size << "\n";

	// If we can divide once more
	if (side_length >= 2) {
		// Add the sites in this block to blocks[side_length]:
		// [
		// (start + system_size*0), (start + system_size*0) + 1, ..., (start + system_size*0) + (side_length - 1),
		// (start + system_size*1), (start + system_size*1) + 1, ..., (start + system_size*1) + (side_length - 1),
		// ...
		// (start + system_size*(side_length-1)), (start + system_size*(side_length-1)) + 1,
		//                                  ... , (start + system_size*(side_length-1)) + (side_length - 1)
		// ]
		for (unsigned i = 0; i <= (side_length - 1); ++i) {
			for (unsigned j = 0; j <= (side_length - 1); ++j) {
				blocks[side_length].push_back((start + i * system_size) + j);

				std::cout << "Adding site " << (start + i * system_size) + j <<
					" to side_length " << side_length << "\n";

			}
		}

		// Find the starts for the rest of the recursive calls:
		// For 2d:
		//     starts = {start + 0 * side_length / 2 + 0 * side_length^2 / 2,,
		//               start + 1 * side_length / 2 + 0 * side_length^2 / 2,
		//               start + 0 * side_length / 2 + 1 * side_length^2 / 2,
		//               start + 1 * side_length / 2 + 1 * side_length^2 / 2,}
		//
		// For nd:
		//     starts = {start + [perm(0, 0, ..., 0)]    .* side_length/2 * [1, system_size, ..., system_size^n],
		//               start + [perm(1, 0, ..., 0)]    .* side_length/2 * [1, system_size, ..., system_size^n],
		//               start + [perm(1, 1, 0, ..., 0)] .* side_length/2 * [1, system_size, ..., system_size^n],
		//               ...
		//               start + [perm(1, 1, 1, ..., 1)] .* side_length/2 * [1, system_size, ..., system_size^n]
		// }
		// where .* is matrix multiplication and perm(...) is all the permutations of ...
		std::vector<unsigned> starts;
		std::vector<unsigned> ground_level_perm(mDimension, 0);
		// perm_counter will keep track on which index should flip to a 1 in the next round in perm(1, 1, 0, ..., 0)
		unsigned perm_counter = 0;
		// There are 2^dimension starting points at each division
		unsigned num_starts = 0;
		while (num_starts < std::pow(2, mDimension)) {
			std::vector<unsigned> all_permutations;
			// Populate all_permutations
			GetPermutations(ground_level_perm, all_permutations);

			// ------------ TEST BLOCK ------------
			std::cout << "Started with perm(";
			for (auto& test_gp : ground_level_perm) {
				std::cout << test_gp << ", ";
			}
			std::cout << ")";
			std::cout << "\nAnd got permutations:" << "\n";
			unsigned test_counter = 0;
			for (auto& test_p : all_permutations) {
				if (test_counter % ground_level_perm.size() == 0) {
					std::cout << "\n";
				}
				std::cout << test_p << ", ";
				test_counter++;
			}
			std::cout << "\n";
			// ------------ /TEST BLOCK ------------

			unsigned next_start = start;
			// Track what side_length^dimension_counter / 2 we are on
			unsigned dimension_counter = 0;
			for (auto& perm_value : all_permutations) {
				// Perform the matrix multiplication
				dimension_counter++;
				next_start = next_start + ((perm_value * side_length * std::pow(system_size, dimension_counter-1)) / 2);

				std::cout << "Adding perm_value * side_length^dimension_counter / 2 to next_start" << "\n";
				std::cout << "perm_value * side_length^dimension_counter / 2:\n" <<
					perm_value <<
					" * " << std::pow(side_length, dimension_counter) <<
					" / 2 = " << ((perm_value * std::pow(side_length, dimension_counter)) / 2) <<
					"\n";
				std::cout << "=> next_start: " << next_start << "\n";

				// all_permutations is just a long list of all permutations after each other
				// therefore, need to split it up into chunks of each permutation
				// Each permutation has the length of ground_level_perm.size()
				if ((dimension_counter % ground_level_perm.size() == 0) && (dimension_counter != 0)) {

					std::cout << "Going to add a new start since dimension_counter % ground_level_perm.size() == 0: " << dimension_counter << " % " << ground_level_perm.size() << "\n";
					std::cout << "=============="<< "\n";
					std::cout << "New start: " << next_start << "\n";
					std::cout << "==============" << "\n";

					// We have moved through one permutation
					starts.push_back(next_start);
					num_starts++;
					// Reset next_start and dimension_counter
					next_start = start;
					dimension_counter = 0;
				}
			}

			std::cout << "Flipping index: " << perm_counter << " in ground_level_perm." << "\n";
			std::cout << "\n\n================================\n\n";

			if (perm_counter < ground_level_perm.size()) {
				// Get the next set of permutations
				ground_level_perm.at(perm_counter) = 1;
				perm_counter++;
			}
		}

		std::cout << "All the starts found: " << "\n";
		for (auto& s : starts) {
			std::cout << s << ", ";
		}
		std::cout << "\n";

		// Call recursively with these starting points and half the side length
		for (auto& s : starts) {
			DivideGraphRec(blocks, side_length/2, s, system_size);
		}
	}
}

/**
* @brief: For each block size, calculate how many blocks are covering the worm
*         Then calculate the box dimension as D_box = log(N_s) / log(1/s)
*         Where s = size and N_s is the number of occupied blocks for that size
*         structure is a collection of sites that should be examined as a fractal object
*
* @param: unordered_map<unsigned std::vector<unsigned>>& blocks
*         std::unordered_map<unsigned, unsigned>& sidelength_and_numoccupied
*         std::vector<unsigned>& structure
*
* @return: void
*/
void Graph::GetBoxDimension(std::unordered_map<unsigned, std::vector<unsigned>>& blocks, std::unordered_map<unsigned, unsigned>& sidelength_and_numoccupied, std::vector<unsigned>& structure) {
	std::cout << "\nCall to GetBoxDimension" << "\n";
	for (auto& side_length_and_sites : blocks) {

		std::cout << "Examining side length: " << side_length_and_sites.first << "\n";

		unsigned num_occupied = 0;
		unsigned index = 0;
		bool occupied = 0;
		for (auto& site : side_length_and_sites.second) {
			// Keep track of how far we've gone
			index++;
			std::cout << "Examining site: " << site << "\n";
			std::cout << "On index: " << index << "\n";

			// If current box not already occupied
			if (!occupied) {
				std::vector<unsigned> neighbours;
				GetLinkedNeighbours(site, neighbours);
				for (auto& neighbour : neighbours) {
					// The site is occupied if it and its connected neighbour are both in structure
					if ((IsInVector(site, structure)) && (IsInVector(neighbour, structure))) {
						// It is connected so this box is occupied
						num_occupied += 1;
						occupied = 1;

						std::cout << "Found an occupied site: " << site << "\n";
						std::cout << "Adding to num_occupied and it's now: " << num_occupied << "\n\n";

						break;
					}
				}
			}

			// Check if we have a new box
			unsigned num_sites_in_box = std::pow(side_length_and_sites.first, mDimension);
			if (index % num_sites_in_box == 0) {
				std::cout << "Setting occupied to 0 since: index % num_sites_in_box == 0: " << index << " % " << num_sites_in_box << "\n";
				occupied = 0;
			}

		}

		// double box_dim = std::log(num_occupied) / std::log(side_length_and_sites.first);
		sidelength_and_numoccupied[side_length_and_sites.first] = num_occupied;

		std::cout << "num_occupied: " << num_occupied << "\n";
		std::cout << "side_length: " << side_length_and_sites.first << "\n";

	}
}
