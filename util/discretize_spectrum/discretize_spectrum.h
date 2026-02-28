/*
 *
 *
 *
 *
 */
#include <iostream>
#include <algorithm>
#include <string>

#include "readwrite.h"

class Srf{
private:
public:
	double wavelength;
	double response;
};

double integrateSrf(int Nsrf, Srf*& srf, double lambda_start, double lambda_end);
