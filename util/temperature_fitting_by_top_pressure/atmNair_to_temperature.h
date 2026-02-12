#include <iostream>
#include <fstream>

#include "interface.h"
#include "wrapper.h"

#include "readwrite.h"

constexpr double ATM_MOL_W {28.9408}; /* MSIS */


ParamAtmosphere* call_temperatures(
	ParamAtmosphere*& rawAtm,
	const int Nheights,
	const int iTOP,
	const double COEF);

ParamAtmosphere* Nair_to_atmosphere_new(
	int atm_Nheights,
	PlanetParam earth,
	double* heights,
	double* x,
	int iTOP,/* iTOP + 1/2 */
	const double PRESSURE_AT_TOP_MID//,
//	double* gm_e_arr
);
ParamAtmosphere* Nair_to_atmosphere_old(
	int atm_Nheights,
	PlanetParam earth,
	double* heights,
	double* x,
	int iTOP,/* iTOP + 1 */
	const double PRESSURE_AT_TOP//,
//	double* gm_e_arr
);

