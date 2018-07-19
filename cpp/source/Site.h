#ifndef SITE_H
#define SITE_H

#include <unordered_map>
#include <vector>

class Site {
public:
	// Constructor/destructor
	Site(unsigned index, unsigned length, unsigned dimension);

	// Fields
	std::unordered_map<unsigned, bool> neighbours;
	unsigned GetIndex();
private:
	unsigned mRootIndex;

	// Functions
	void AddNeighbours(unsigned length, unsigned dimension);
	unsigned CalcIndexFromVector(std::vector<unsigned>& xyz, unsigned length);
};

#endif
