/* 
 * LookingDirection.cpp
 *
 */


#include"coordinate.h"

LookingDirection::LookingDirection(const double Alpha, const double Alt){
	set(Alpha, Alt);
}

void LookingDirection::set(const double Alpha, const double Alt){
	pAltitude = Alt;
	pAlpha = Alpha;
}


Geocoordinate LookingDirection::tangential_point(PlanetParam earth, SatelliteParam satellite){
	double R = earth.radius() + pAltitude;
	double cos_b = R / satellite.radius();
	double sin_b = std::sqrt( 1.0 - cos_b*cos_b );
	AndoLab::Vector3d <double> r( R*cos_b, 0.0, R*sin_b );

	return Geocoordinate(earth, satellite, rotate_alpha( r, pAlpha*Deg2rad ));
	//  return AndoLab::Vector3d <double> (
	//      R*cos_b, -R*sin_b*sin(alpha), R*sin_b*cos(alpha) );
}
	
