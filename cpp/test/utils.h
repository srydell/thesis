#ifndef UTILS_H
#define UTILS_H

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
