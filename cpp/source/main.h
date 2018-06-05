#ifndef MAIN_H
#define MAIN_H

#include <unordered_map>

void UpdateCorrelationFunction(unsigned site0, unsigned site1, std::unordered_map<unsigned, unsigned> &correlation_func);
bool IsAccepted(double K, bool link_between, long double &random_num);

#endif
