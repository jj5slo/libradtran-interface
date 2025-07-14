#include <cmath>
#include <Vector3d.h>

#include "coordinate.h"
/* TODO satellite.latitude() も反映するようにする */
Geocoordinate::Geocoordinate(
		PlanetParam earth, SatelliteParam satellite, AndoLab::Vector3d <double> r){
	set(earth, satellite, r);
}

Geocoordinate::Geocoordinate(
		PlanetParam earth, SatelliteParam satellite, const double Lat, const double Long, const double Alt){
	set(earth, satellite, Lat, Long, Alt);
}

void Geocoordinate::set(PlanetParam earth, SatelliteParam satellite, AndoLab::Vector3d <double> r){
	pR = r;

	pAltitude = r.r() - earth.radius();

	/* φは東経140.7°が基準になっている */
	double Phi = r.phi() + satellite.longitude()*Deg2rad;
	if ( Phi > M_PI ){
		Phi -= 2.0*M_PI;
	}
	pLongitude = Phi * Rad2deg;

	pLatitude = ( M_PI/2.0 - r.theta() ) * Rad2deg;
}

void Geocoordinate::set(
		PlanetParam earth, SatelliteParam satellite, const double Lat, const double Long, const double Alt){

	pLatitude = Lat;
	pLongitude = Long;
	pAltitude = Alt;

	const double r = pAltitude + earth.radius();
	const double th = (90.0 - Lat) * Deg2rad;
	double ph = (Long - satellite.longitude()) * Deg2rad;
	if ( ph < -M_PI ){
		ph += 2.0*M_PI;
	}

	pR.set(r, th, ph, AndoLab::coordinate::Spherical);
}

/* pRの点を、衛星(x軸上)から見て、yz平面へ !! 視線方向へ !! 射影した点 */
AndoLab::Vector3d <double> Geocoordinate::projection_on_yz(SatelliteParam satellite){

	AndoLab::Vector3d <double> r_geo { satellite.radius(), 0.0, 0.0 }; /* ひまわりの位置 */

	/* r0 = R r^(θ, φ)
	 * d^ = (r0 - Rgeo)/|r0 - Rgeo|
	 * (Rgeo + t d^)・x^ = 0 となるtを探す
	 * Rgeo.x + t d^.x = 0 ←→ t = - Rgeo.x / d^.x;
	 * (Rgeo + t d^)・z^ / |Rgeo + t d^| = cosα
	 **/

	AndoLab::Vector3d <double> dn = (pR - r_geo).n();
	double t = - r_geo.x() / dn.x();
	return r_geo + t * dn;
}


double Geocoordinate::alpha(SatelliteParam satellite){
	/* ひまわりから見てyz軸上へ射影した点の、z軸からの角度α
	 * -y方向(ヨーロッパ側)がαが正、y方向(アメリカ側)がαは負
	 * 　　ｚ
	 * ＼α↑
	 * 　＼│
	 * ──┴─→y
	 */
	AndoLab::Vector3d <double> yz_pts = projection_on_yz(satellite);
	return std::atan2( -yz_pts.y(), yz_pts.z() );
}


double Geocoordinate::east_longitude(void){
	double east_longi;

	if ( pLongitude > 0.0 ){ /* 東経のときはそのまま返す */
		east_longi = pLongitude;
	} else { /* 西経のとき（負のとき）は、-180°からの距離を180°に足す */
		east_longi = 180.0 + ( pLongitude - (-180.0) );
	}
	return east_longi;
}
