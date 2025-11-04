#include "wrapper.h"

void set_stdin_direction(
	const int index,
	ParamStdin &pStdin,/* 参照渡し(かポインタ)でないといけない */
	const Geocoordinate* tparr,
	const PlanetParam earth,/* これは楕円とかで変わる可能性もある */
	const SatelliteParam himawari,
	const double maxHeight,
	const int doy,
	const obsDateTime dt/* 一時刻しかここでは扱わない */
){

	Geocoordinate tp = tparr[index];
	std::cout << "Tangential point:\n";
	std::cout << "\tlat:" << tp.latitude() << "\n\tlon:" << tp.longitude() << "\nt\taltitude:" << tp.altitude() << "\n\talpha:" << tp.alpha()*Rad2deg << std::endl;


	AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
	crosspts = Across_point_atmosphere(earth, himawari, tp.r(), maxHeight); /* maxHeight=TOA */
	Geocoordinate crosspt(earth, himawari, crosspts[0]);/* TOAと視線の交点を求める */

	std::cout << "crosspoint_of_atmosphere:\n\tlat:" << crosspt.latitude() << "\n\tlon:" << crosspt.longitude() << "\n\taltitude:" << crosspt.altitude() << std::endl;
	
	AndoLab::solar_direction(crosspt.latitude(), crosspt.longitude(), doy, dt.Hour(), &pStdin.sza, &pStdin.phi0);/* tangential point での太陽方向を求める */
	std::cout << "sza:" << pStdin.sza << " phi0:" << pStdin.phi0 << std::endl;
	sensor_direction(himawari, crosspt.r(), &pStdin.umu, &pStdin.phi);/* crosspt からみた衛星方向を元に、視線方向の局所鉛直からの角（オフナディア角）を求める */
	double sensor_theta = acos(pStdin.umu) * Rad2deg;
	std::cout << "sonsor_direction:\n\tsensor_theta:" << sensor_theta <<"\n\tphi:" << pStdin.phi << std::endl;
	std::cout << "cos(sensor_theta) = umu = " << pStdin.umu << std::endl;

}
