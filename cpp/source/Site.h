#ifndef SITE_H
#define SITE_H

#include <unordered_map>

class Site
{
public:
	// Constructor/destructor
	Site (int index, int length);

	// Fields
	std::unordered_map<int, bool> neighbours;
private:
	int rootIndex;

	// Functions
	void addNeighbours(int rootIndex, int length, std::unordered_map<int, bool> neighbours);
};

#endif
