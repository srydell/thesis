#include <cmath>
#include <unordered_map>

/**
* @brief: Return -1 if -site in neighbours_to_site0 and +1 if +site
*
* @param: const std::unordered_map<int, int>& neighbours_to_site0
*         int site0
*         int site1
*         int dimension
*         int length
*
* @return: int
*/
int GetSign(const std::unordered_map<int, int>& neighbours_to_site0, int site0, int site1, int dimension, int length) {
	if (site0 == 0) {
		for (int d = 0; d < dimension; ++d) {
			if (site1 == std::pow(length, d)) {
				return +1;
			} else if (site1 == (length - 1) * std::pow(length, d)) {
				return -1;
			}
		}
	}
	for (auto& link : neighbours_to_site0) {
		if (link.first == site1) {
			return +1;
		}
		if (link.first == -1 * site1) {
			return -1;
		}
	}
	throw "Found no sign";
}
