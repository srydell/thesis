#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <ostream>

/**
* @brief: Write to ostream out the contents of container c with delimiter '\n' by default
*
* @param: const Container& c
*         std::ostream& out
*         char delimiter = '\n'
*
* @return: None
*/
template<class Container>
std::ostream& write_container(const Container& c, std::ostream& out, char delimiter = '\n') {
    bool write_sep = false;
    for (const auto& e: c) {
        if (write_sep)
            out << delimiter;
        else
            write_sep = true;
        out << e;
    }
    return out;
}

/**
* @brief: Populate vector_to_store_perms_in with all the permutations of values in vector_to_permute
*
* @param: std::vector<T> vector_to_permute
*         std::vector<T>& vector_to_store_perms_in
*
* @return: None
*/
template<typename T>
void GetPermutations(std::vector<T> vector_to_permute, std::vector<T>& vector_to_store_perms_in) {
	sort(vector_to_permute.begin(), vector_to_permute.end());
	do {
		for (auto& i : vector_to_permute) {
			vector_to_store_perms_in.push_back(i);
		}
	} while (std::next_permutation(vector_to_permute.begin(), vector_to_permute.end()));
}

/**
* @brief: Return true if item exist in map
*
* @param: template typename T1 item
*       : template typename T1 &map
*
* @return: bool
*/
template<typename T1, typename T2>
bool HasItem(T1 item, T2 &map) {
	// Try with unordered_map like behaviour
	return map.find(item) != map.end();
}

#endif
