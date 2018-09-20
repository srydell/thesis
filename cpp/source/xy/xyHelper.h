#ifndef XYHELPER_H
#define XYHELPER_H

struct WNandNS{
	WNandNS(double winding_number, long double num_steps) : winding_number(winding_number), num_steps(num_steps) {};
	double winding_number;
	long double num_steps;
};

#endif
