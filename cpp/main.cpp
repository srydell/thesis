#include <iostream>
#include <map>
#include <set>
#include <string>
#include <unordered_map>

int main(){
	/* std::map<std::string, int> wordMap; */

	/* // Insert Few elements in map */
	/* wordMap.insert({ "First", 1 }); */
	/* wordMap.insert({ "Second", 2 }); */
	/* wordMap.insert({ "Third", 3 }); */

	/* // Overwrite value of an element */
	/* wordMap["Third"] = 8; */

	/* // Iterate over the map and display elements */
	/* for (std::pair<std::string, int> element : wordMap) */
	/* 	std::cout << element.first << " :: " << element.second << std::endl; */

	/* // Graph with (i, j) : int */
	/* std::map<std::set<int>, int> mockGraph; */
	/* std::set<int> site; */
	/* site.insert(1); */
	/* site.insert(3); */

	/* mockGraph.insert({ site, 2 }); */

	/* for (std::pair<std::set<int>, int> element : mockGraph){ */
	/* 	/1* Get the x/y data *1/ */
	/* 	int x = *element.first.begin(); */
	/* 	int y = *element.first.end(); */
	/* 	std::cout << '(' << x << ", " << y << ')' << " : " << element.second << std::endl; */
	/* 	std::set<int>::iterator siteIndex; */
	/* 	for (siteIndex=element.first.begin(); siteIndex!=element.first.end(); ++siteIndex) { */
	/* 		std::cout << *siteIndex << " : " << element.second << std::endl; */
	/* 	} */
	/* } */

	/* // Graph with {(i, j) : {(i+1, j) : 0, ...}, ...} */
	/* typedef std::unordered_map<std::set<int>, int> map1; */
	/* typedef std::unordered_map<std::set<int>, map1> map2; */
	/* map2 unordered_graph; */

	// This is what I will use. Use a StringStream to turn ints into strings to then be used as keys.
	std::unordered_map<std::string, std::unordered_map<std::string, int>> vertices;

	std::map<std::set<int>, std::map<std::set<int>, int>> graph;
	std::set<int> testSite;
	testSite.insert(1);
	testSite.insert(3);

	/* Mockup of an iteration over site */
	std::set<int>::iterator it;
	for (it=testSite.begin(); it!=testSite.end(); ++it) {
		std::cout << ' ' << *it;
	}
	std::cout << '\n';


	std::set<int> testNeighbour1;
	testNeighbour1.insert(2);
	testNeighbour1.insert(3);

	std::set<int> testNeighbour2;
	testNeighbour2.insert(1);
	testNeighbour2.insert(2);

	/* starting weight for every member */
	int startingValue = 0;

	std::map<std::set<int>, int> neighbours;

	/* Populate the neighbours map */
	neighbours.insert({ testNeighbour1, startingValue });
	neighbours.insert({ testNeighbour2, startingValue });

	graph.insert({ testSite, neighbours });

	for (std::pair<std::set<int>, std::map<std::set<int>, int>> element : graph){
		/* {    (1, 3) :  {     (2, 3)  :  0, ...}, ...} */
		/* map< set<int>, map< set<int>, int > > */
		/* Get the x/y data */
		int x = *element.first.begin();
		int y = *element.first.end();
		std::cout << "The current site is:" << std::endl;
		std::cout << '(' << x << ", " << y << ')' << std::endl;

		/* Iterate over the current site indices */
		std::cout << "Iterating over the current site indices." << std::endl;
		std::set<int>::iterator siteIndex;
		for (siteIndex=element.first.begin(); siteIndex!=element.first.end(); ++siteIndex) {
			std::cout << "Site index: " << *siteIndex << std::endl;
		}

		/* Iterate over the neighbours */
		std::cout << "Iterating over the neighbours." << std::endl;
		for (std::pair<std::set<int>, int> neighbourData : element.second){
			std::cout << "(";
			std::set<int>::iterator siteIndex;
			for (siteIndex = neighbourData.first.begin(); siteIndex!=neighbourData.first.end(); ++siteIndex) {
				std::cout << ' ' << *siteIndex;
			}
			std::cout << " )" << std::endl;
		}
	}

}
