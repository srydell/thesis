#include <iostream>
#include <regex>
#include <string>
#include <sstream> 

/**
* @brief: brief
*
* @param: int site0
*       : int site1
*       : int link
*
* @return: void
*/
inline void SetLinks(int site0, int site1, int link) {
	if (link < 0) {
		int tmp = site0;
		site0 = site1;
		site1 = tmp;
		link = -1 * link;
	}
	for (int i = 0; i < link; ++i) {
		std::cout << "Switch link between " << site0 << " and " << site1 << "\n";
	}
}

int main() {
	std::string input = "0: 1,1 3,-1 15,-2";
	std::cout << "Input is: " << input << "\n";
	// std::regex sites_and_links("(\\d+): (-?\\d+),(-?\\d+) (-?\\d+),(-?\\d+)");
	std::regex sites_and_links;
	// This block sets up the number of (\d,\d) needed
	{
		std::stringstream ss;
		ss << "(\\d+):";
		size_t n = std::count(input.begin(), input.end(), ',');
		for (size_t i = 0; i < n; ++i) {
			ss << " ";
			ss << "(-?\\d+),(-?\\d+)";
		}
		std::cout << "Got the regex: " << ss.str() << "\n";
		// Create the resulting regex
		sites_and_links = ss.str();
	}

	std::smatch matches;
	if (std::regex_search(input, matches, sites_and_links)) {
		int current_site;
		if (matches.size() > 1) {
			std::stringstream(matches[1].str()) >> current_site;
		}
		for (size_t i = 2; i < matches.size(); ++i) {
			// TODO: Actually switch the sites
			int neighbour, link;
			std::stringstream(matches[i].str()) >> neighbour;
			std::stringstream(matches[++i].str()) >> link;
			std::cout << "\n================================" << "\n";
			std::cout << "Swithing site between " << current_site << " and " << neighbour << "\n";
			// std::cout << "With the weight: " << link << "\n\n";
			SetLinks(current_site, neighbour, link);
		}
	}

}
