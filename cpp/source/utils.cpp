#include <unordered_map>

/**
* @brief: Return true if key exist in map
*
* @param: unsigned key
*       : std::unordered_map<unsigned, unsigned> &map
*
* @return: bool
*/
bool HasKey(unsigned key, std::unordered_map<unsigned, unsigned> &map) {
	return map.find(key) == map.end();
}
