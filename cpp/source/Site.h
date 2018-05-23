#ifndef SITE_H
#define SITE_H

#include <unordered_map>

class Site
{
public:
	// Constructor/destructor
	Site (unsigned index, unsigned length);

	// Fields
	std::unordered_map<unsigned, bool> neighbours;
private:
	unsigned rootIndex;

	// Functions
	void addNeighbours(unsigned rootIndex, unsigned length);
};

#endif
