#ifndef XYHELPERS_H
#define XYHELPERS_H

#include <unordered_map>

int GetSign(const std::unordered_map<int, int>& neighbours_to_site0, int site0, int site1, int dimension, int length);

#endif
