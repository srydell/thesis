#include "Graph.h"
#include "utils.h"
#include <cmath>
#include <iostream>
#include <random>
#include <sstream>
#include <vector>

/**
* @brief: Return -1 if -site1 in neighbours to site0 and +1 if +site1
*
* @param: int site0
*         int site1
*
* @return: int
*/
int Graph::GetSign(int site0, int site1) {

	// Function assumes site0/1 >= 0
	site0 = site0 < 0 ? -1 * site0 : site0;
	site1 = site1 < 0 ? -1 * site1 : site1;
	// std::cout << "Looking for sign between: " << site0 << " and " << site1 << "\n";

	// To avoid dealing with both edge cases (site0 == 0 && site1 == 0)
	int reversed = site1 == 0 ? -1 : 1;
	if (reversed == -1) {
		// std::cout << "Switching site0 and site1 since site1 == 0" << "\n";
		int tmp = site0;
		site0 = site1;
		site1 = tmp;
		// std::cout << "(site0, site1) = (" << site0 << ", " << site1 << ")" << "\n";
	}

	// Special case for 0 since it 'does not have a sign'
	if (site0 == 0) {
		// std::cout << "site0 is zero" << "\n";
		for (int d = 0; d < mDimension; ++d) {
			if (site1 == std::pow(mLength, d)) {
				// std::cout << "site1 is L^d: " << mLength << "^" << d << " = " << site1 << "\n";
				// std::cout << "Returning " << -1 * reversed << "\n";
				return +1 * reversed;
			} else if (site1 == (mLength - 1) * std::pow(mLength, d)) {
				// std::cout << "site1 is (L - 1) * L^d: (" << mLength << " - 1) * " << mLength << "^" << d << " = " << site1 << "\n";
				// std::cout << "Returning " << -1 * reversed << "\n";
				return -1 * reversed;
			}
		}
	}

	// std::cout << "site0 is not zero" << "\n";

	auto neighbours_to_site0 = mGraph[site0].neighbours;
	for (auto& link : neighbours_to_site0) {
		// std::cout << "On link: (" << link.first << ", " << link.second << ")\n";
		if (link.first == site1) {
			// std::cout << "Returning +1" << "\n";
			return +1;
		}
		if (link.first == -1 * site1) {
			// std::cout << "Returning -1" << "\n";
			return -1;
		}
	}
	throw "Found no sign";
}

/**
* @brief: Change value between site0 and site1 in mGraph so that if positive link ++ otherwise --
*
* @param: int site0
*       : int site1
*
* @return: void
*/
void Graph::SwitchLinkBetween(int site0, int site1) {
	// std::cout << "Switching link between: " << site0 << " and " << site1 << "\n";
	// Check if sites are in mGraph
	if (IsInGraph(site0) && IsInGraph(site1)) {
		if (AreNeighbours(site0, site1)) {

			int site0_to_site1_sign = GetSign(site0, site1);
			mGraph[site0].neighbours[site0_to_site1_sign * site1]++;
			mGraph[site1].neighbours[-1 * site0_to_site1_sign * site0]--;

			// std::cout << "Switch between site0: " << site0 << " and site1: " << site1 << "\n";
			// std::cout << "mGraph[" << site0 << "].neighbours[" << site0_to_site1_sign * site1 << "]++;" << "\n";
			// std::cout << "mGraph[" << site1 << "].neighbours[" << -1 * site0_to_site1_sign * site0 << "]--;" << "\n\n";

		} else {
			// Error handling for siteX and siteY not neighbours
			std::stringstream ss;
			ss << "Call to Graph::SwitchLinkBetween failed since "
				<< site0 << " and " << site1 << " are not in neighbours." << "\n";
			throw ss.str();
		}

	} else {
		// Error handling for site not in mGraph
		std::stringstream ss;
		ss << "Call to Graph::SwitchLinkBetween failed since "
			<< site0 << " and/or " << site1 << " are not in mGraph." << "\n";
		throw ss.str();
	}
}

/**
* @brief: Return the link between site0 and site1 if it exists. Otherwise throw std::string error.
*         NOTE: site0 and site1 is assumed to be > 0
*         NOTE: This is the physical link, not necessarily the link from site0 to site1
*               If the arrow is in the negative x_i direction, the link will be negative, otherwise positive
*
* @param: int site0
*       : int site1
*
* @return: int
*/
int Graph::GetLink(int site0, int site1) {
	if (IsInGraph(site0) && IsInGraph(site1)) {
		if (AreNeighbours(site1, site0)) {
			int sign = GetSign(site0, site1);

			// Example:
			//         s0 {s1  :  J}
			//         s1 {s0  :  J}
			//         -------------
			//     Should return +1:
			//         11 {-10 : -1}
			//         10 {11  : +1}
			//     Should return -1:
			//         11 {-10 : +1}
			//         10 {11  : -1}
			return sign * mGraph[site0].neighbours[sign * site1];
		} else {
			// Error handling for site1 not neighbour to site0
			std::stringstream ss;
			ss << "Call to Graph::GetLink failed since "
				<< site1 << " is not a neighbour to " << site0 << "\n";
			throw ss.str();
		}
	} else {
		// Error handling for site0 not in mGraph
		std::stringstream ss;
		ss << "Call to Graph::GetLink failed since one of "
			<< site0 << " or/and " << site1 << " is not in mGraph" << "\n";
		throw ss.str();
	}
}

/**
* @brief: Check mGraph if site has neighbour
*
* @param: int site
*       : int neighbour
*
* @return: bool
*/
bool Graph::AreNeighbours(int site0, int site1) {
	for (auto& neighbour : mGraph[site0].neighbours) {
		// Assume here that if site0 has site1 then the other way too
		if ((neighbour.first == site1) || (neighbour.first == -1 * site1)) {
			return 1;
		}
	}
	return 0;
}

/**
* @brief: brief
*
* @param: int site0
*       : int site1
*       : int link
*
* @return: void
*/
void Graph::SetLinks(int site0, int site1, int link) {
	// std::cout << "Switching link between: " << site0 << " and " << site1 << "\n";
	// Check if sites are in mGraph
	if (IsInGraph(site0) && IsInGraph(site1)) {
		if (AreNeighbours(site0, site1)) {

			int site0_to_site1_sign = GetSign(site0, site1);
			mGraph[site0].neighbours[site0_to_site1_sign * site1] = link;
			mGraph[site1].neighbours[-1 * site0_to_site1_sign * site0] = -1 * link;

			// std::cout << "Switch between site0: " << site0 << " and site1: " << site1 << "\n";
			// std::cout << "mGraph[" << site0 << "].neighbours[" << site0_to_site1_sign * site1 << "]++;" << "\n";
			// std::cout << "mGraph[" << site1 << "].neighbours[" << -1 * site0_to_site1_sign * site0 << "]--;" << "\n\n";

		} else {
			// Error handling for siteX and siteY not neighbours
			std::stringstream ss;
			ss << "Call to Graph::SwitchLinkBetween failed since "
				<< site0 << " and " << site1 << " are not in neighbours." << "\n";
			throw ss.str();
		}

	} else {
		// Error handling for site not in mGraph
		std::stringstream ss;
		ss << "Call to Graph::SwitchLinkBetween failed since "
			<< site0 << " and/or " << site1 << " are not in mGraph." << "\n";
		throw ss.str();
	}
}
