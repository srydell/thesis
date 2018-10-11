#include <iostream>
#include <regex>
#include <string>
#include <sstream> 

int main() {
	// ss << "Energy: " << data.energy << ", " << "Winding Number: " << data.winding_number;
	std::string input = "Energy: -12.3241, Winding Number: 4.2";

	std::cout << "Input is: " << input << "\n";
	// std::regex energy_winding("(\\d+): (-?\\d+),(-?\\d+) (-?\\d+),(-?\\d+)");
	std::regex energy_winding;
	// This block sets up the number of (\d,\d) needed
	{
		std::stringstream ss;
		ss << "Energy: ";
		ss << "(-?\\d*\\.?\\d*)";
		ss << ", Winding Number: ";
		ss << "(-?\\d*\\.?\\d*)";
		std::cout << "Got the regex: " << ss.str() << "\n";
		// Create the resulting regex
		energy_winding = ss.str();
	}

	std::smatch matches;
	if (std::regex_search(input, matches, energy_winding)) {
		if (matches.size() > 1) {
			double energy;
			double winding_number;
			std::stringstream(matches[1].str()) >> energy;
			std::stringstream(matches[2].str()) >> winding_number;
			std::cout << "Got the energy: " << energy << "\n";
			std::cout << "Got the winding_number: " << winding_number << "\n";
		}
	}
}
