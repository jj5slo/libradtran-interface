#include<iostream>

#include "coordinate.h"
#include "readwrite.h"


double lat0s[88] = {38.0, 39.0, 40.0, 41.0, 42.0, 43.0, 44.0, 45.0, 46.0, 47.0, 48.0, 49.0, 50.0, 51.0, 52.0, 53.0, 54.0, 55.0, 56.0, 57.0, 58.0, 59.0, 60.0, 61.0, 62.0, 63.0, 64.0, 65.0, 66.0, 67.0, 68.0, 69.0, 70.0, 71.0, 72.0, 73.0, 74.0, 75.0, 76.0, 77.0, 78.0, 79.0, 80.0, 81.0, 81.0, 80.0, 79.0, 78.0, 77.0, 76.0, 75.0, 74.0, 73.0, 72.0, 71.0, 70.0, 69.0, 68.0, 67.0, 66.0, 65.0, 64.0, 63.0, 62.0, 61.0, 60.0, 59.0, 58.0, 57.0, 56.0, 55.0, 54.0, 53.0, 52.0, 51.0, 50.0, 49.0, 48.0, 47.0, 46.0, 45.0, 44.0, 43.0, 42.0, 41.0, 40.0, 39.0, 38.0};
double lon0s[88] = {61.8, 62.0, 62.2, 62.3, 62.5, 62.7, 62.9, 63.1, 63.4, 63.6, 63.8, 64.1, 64.4, 64.7, 65.0, 65.3, 65.7, 66.1, 66.5, 66.9, 67.4, 67.9, 68.4, 69.0, 69.6, 70.3, 71.0, 71.8, 72.7, 73.6, 74.6, 75.8, 77.1, 78.5, 80.2, 82.0, 84.2, 86.7, 89.7, 93.3, 97.8, 103.7, 112.3, 128.8, 152.6, 169.1, 177.7, 183.6, 188.1, 191.7, 194.7, 197.2, 199.4, 201.2, 202.9, 204.3, 205.6, 206.8, 207.8, 208.7, 209.6, 210.4, 211.1, 211.8, 212.4, 213.0, 213.5, 214.0, 214.5, 214.9, 215.3, 215.7, 216.1, 216.4, 216.7, 217.0, 217.3, 217.6, 217.8, 218.0, 218.3, 218.5, 218.7, 218.9, 219.1, 219.2, 219.4, 219.6};

double toa = 100.0;/* km */

int main(void){

	auto earth = PlanetParam(6370.e3);
	auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
	
	double** output = new double*[9];/* lineno, lat, lon, TPlat, TPlon, CPlat0, CPlon0, CPlat1, CPlon1 */
	for(int i=0; i<9; i++){
		output[i] = new double[88];
	}
	for(int i=0; i<88; i++){
		Geocoordinate on_ground(earth, himawari, lat0s[i], lon0s[i], 0.0);
		double ld_alpha = on_ground.alpha()*Rad2deg;

		LookingDirection ld(ld_alpha, 0.0);
		Geocoordinate tp = ld.tangential_point(earth, himawari);
	
		AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
		crosspts = Across_point_atmosphere(earth, himawari, tp.r(), toa); 
		Geocoordinate crosspt0(earth, himawari, crosspts[0]);/* TOAと視線の交点を求める */
		Geocoordinate crosspt1(earth, himawari, crosspts[1]);/* TOAと視線の交点を求める */
		
		output[0][i] = i+1;
		output[1][i] = lat0s[i];
		output[2][i] = lon0s[i];
		output[3][i] = tp.latitude();
		output[4][i] = tp.longitude();
		output[5][i] = crosspt0.latitude();
		output[6][i] = crosspt0.longitude();
		output[7][i] = crosspt1.latitude();
		output[8][i] = crosspt1.longitude();
	}
	std::string header = "# toa: " + std::to_string(toa) + " km\n"
		+ "# lineno, lat, lon, TPlat, TPlon, CPlat0, CPlon0, CPlat1, CPlon1\n";

	readwrite::save_data("./output.dat", header, 88, 9, output);
	
	return 0;
}
