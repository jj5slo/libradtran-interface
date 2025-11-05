/* MSIS_to_GM_E.cpp
 */
#include "wrapper.h"

double* msis_to_gm_e(
	ParamAtmosphere* patm,
	PlanetParam earth,
	obsDateTime dt,
	Geocoordinate* coord,
	int Nheights
){
	double* gm_e = new double[Nheights - 1];
	double height;
	double radius;
	for(int i=0; i<Nheights - 1; i++){
		height = (patm[i].z + patm[i+1].z) / 2.0 * 1.0e3;/* [m] */
		double air_mass_density = get_msis_total_mass_density( dt, coord[i] , coord[i+1]) * 1.0e3;/* [kg m-3] */
		radius = height + earth.radius();
		double dp = (patm[i+1].p - patm[i].p) * 1.0e2;
		double dz = (patm[i+1].z - patm[i].z) * 1.0e3;
		gm_e[i] = - dp/dz/air_mass_density * radius*radius;
	}	
	return gm_e;
}
