#ifndef UTILS_H
#define UTILS_H

#include <algorithm>
#include <iostream>
#include <ostream>
#include <vector>

/**
* @brief: Return the values inbetween v_to_slice[start_index] and v_to_slice[end_index] as a vector
*
* @param: std::vector<T> const v_to_slice
*       : int start_index
*       : int end_index
*
* @return: std::vector<T>
*/
template<typename T>
std::vector<T> slice(std::vector<T> const &v_to_slice, int start_index, int end_index) {
	auto first = v_to_slice.cbegin() + start_index;
	auto last = v_to_slice.cbegin() + end_index + 1;

	std::vector<T> out_v(first, last);
	return out_v;
}

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
