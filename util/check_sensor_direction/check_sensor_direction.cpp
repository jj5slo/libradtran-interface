#include <iostream>

#include "FixedParams.h"
#include "coordinate.h"

#include "latlon.h"

int main(void){
	
	auto earth = PlanetParam( 6370.e3 );
	auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
	
	std::cout << "#lineno lat lon ld_alpha offnadir_on_ground(=const) looking_azimuth_on_ground" << std::endl;

	for(int i=0; i<88; ++i){
		Geocoordinate on_ground(earth, himawari, lat[i], lon[i], 0.0);/* 観測データにある緯度経度の高度0km 地点のGeocoordinate */
		double ld_alpha = on_ground.alpha()*Rad2deg;
		
		LookingDirection ld;
		ld.set(ld_alpha, 0.0);
		Geocoordinate tp = ld.tangential_point(earth, himawari);

		AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
		crosspts = Across_point_atmosphere(earth, himawari, tp.r(), 100.0); 
		Geocoordinate crosspt(earth, himawari, crosspts[0]);/* TOAと視線の交点を求める */
		double offnadir_on_ground = 0.0;
		double looking_azimuth_on_ground = 0.0;

		sensor_direction(himawari, crosspt.r(), &offnadir_on_ground, &looking_azimuth_on_ground);
		offnadir_on_ground = acos(offnadir_on_ground)*Rad2deg;
		std::cout << (i+1) <<" "<< lat[i] <<" "<< lon[i] <<" "<< ld_alpha <<" "<< offnadir_on_ground <<" "<< looking_azimuth_on_ground << std::endl;

	}


	return 0;
}
