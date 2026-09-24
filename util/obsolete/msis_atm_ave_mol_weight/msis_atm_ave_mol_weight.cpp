#include <iostream>

#include "get_msis.h"


int main(void){
	constexpr int Nheights = 101;
	constexpr double lon = 82.0;
	constexpr double lat = 73.0;
	obsDateTime dt;
	dt.set(2022, 6, 1, 3, 0, 0);
	
	auto earth = PlanetParam( 6370.e3 );
	auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );

	Geocoordinate* coords = new Geocoordinate[Nheights];
	for(int i=0; i<Nheights; i++){
		coords[i].set(earth, himawari, lat, lon, (double)i*1.0e3);
	}
	
	double* avr_mol_weights = new double[Nheights];
	for(int i=0; i<Nheights; i++){
		avr_mol_weights[i] = get_msis_average_molecular_weight(dt, coords[i]);
		std::cout << coords[i].altitude() <<" "<< avr_mol_weights[i] << std::endl;
	}
	return 0;
}


