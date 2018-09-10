#ifndef SITE_H
#define SITE_H

#include <unordered_map>
#include <vector>

class Site {
public:
	Site(int index, int length, int dimension);

	// Fields
	std::unordered_map<int, int> neighbours;
	int GetIndex();
private:
	int mRootIndex;

	// Functions
	void AddNeighbours(int length, int dimension);
	int CalcIndexFromVector(std::vector<int>& xyz, int length);
};

#endif
