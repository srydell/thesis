#ifndef XYHELPER_H
#define XYHELPER_H

struct WNENS{
	WNENS(double winding_number, double energy, long double num_steps)
		: winding_number(winding_number), energy(energy), num_steps(num_steps) {};
	double winding_number;
	double energy;
	long double num_steps;
};

#endif
