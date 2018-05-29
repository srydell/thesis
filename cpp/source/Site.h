#ifndef SITE_H
#define SITE_H

#include <unordered_map>

class Site {
public:
	// Constructor/destructor
	Site (unsigned index, unsigned length);

	// Fields
	std::unordered_map<unsigned, bool> neighbours;
	unsigned GetIndex();
private:
	unsigned mRootIndex;

	// Functions
	void AddNeighbours(unsigned length);
};

#endif
