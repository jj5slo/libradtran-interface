#include<iostream>
#include<cstdlib>
#include<algorithm>/* 最大最小とか用 */

#include"solar_direction.h"

#include"coordinate.h"
#include"interface.h"
#include"execute.h"

constexpr int NLINES = 3;

int main(int argc, char *argv[]){
	
	if(argc != 4){
		std::cerr << "Usage: ./main YEAR MONTH DAY\n";
		return 0;
	}
	const int YEAR = atoi(argv[1]);
	const int MONTH = atoi(argv[2]);
	const int DAY = atoi(argv[3]);
	
	obsDateTime dt(YEAR, MONTH, DAY);
	std::string path_obs = obs_path("../", dt);//"../h08_b01_s01s02_20220501_171000.txt";
	int Nobs = 0;
	int obs_index = 50;
	Observed *obsds = read_obs( &Nobs, path_obs );
	std::cout << Nobs << "points" << std::endl;

	double *heights = obsds[obs_index].Heights();
	int Nheights = obsds[obs_index].Nheights();
	std::cout << "lat" << obsds[obs_index].Latitude() << " "  << "lon" << obsds[obs_index].Longitude() << " " << obsds[obs_index].Nheights() << "heights" << std::endl;
	for(int i=0; i<obsds[obs_index].Nheights(); i++){
		std::cout << heights[i] << " " << obsds[obs_index].Data(heights[i]) << "\n";
	}
	std::cout << std::endl;
	
	auto earth = PlanetParam( 6370.e3 );
	auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );

	int doy = 213;/* tekitou */
	/*
	double *sza = new double;
	double *phi0 = new double;
	double *mu = new double;
	double *phi = new double;
	*/
	ParamStdin pstdin;
	pstdin.mc_photons = 300000;/* default is 300000 */
	pstdin.solver = "mystic";
	pstdin.additional = "output_user uu";//\npseudospherical";
/*
	std::cout << &pstdin << " " << &pstdin.sza << std::endl;
	return 0;
*/
	
	double sensor_theta;
	double *radiance = new double [Nheights];
	
	std::string path_stdin = "in";
	std::string path_stdout = "out";
	std::string path_result = "result.txt";

	pstdin.wavelength = 470.0;
	
	Geocoordinate on_ground(earth, himawari, obsds[obs_index].Latitude(), obsds[obs_index].Longitude(), 0.0);/* 観測データにある緯度経度の高度0km 地点のGeocoordinate */
	double ld_alpha = on_ground.alpha(himawari)*Rad2deg;
	
	std::cout << "ld_alpha : " <<  ld_alpha << std::endl;

for(int i=0; i<Nheights; i++){
		LookingDirection ld( ld_alpha, heights[i]/m2km );/* 見る場所決め */
		Geocoordinate tp = ld.tangential_point( earth, himawari );/* 見る場所が実際どの座標なのか？ */
		std::cout << "Tangential point:\n";
		std::cout << "\tlat:" << tp.latitude() << "\n\tlon:" << tp.longitude() << "\nt\taltitude:" << tp.altitude() << "\n\talpha:" << tp.alpha(himawari)*Rad2deg << std::endl;
		
		AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
		crosspts = Across_point_atmosphere(earth, himawari, tp.r());
		Geocoordinate crosspt(earth, himawari, crosspts[0]);/* TOAと視線の交点を求める */
		std::cout << "crosspoint_of_atmosphere:\n\tlat:" << crosspt.latitude() << "\n\tlon:" << crosspt.longitude() << "\n\taltitude:" << crosspt.altitude() << std::endl;
		
		AndoLab::solar_direction(crosspt.latitude(), crosspt.longitude(), doy, 12.0-4.0, &pstdin.sza, &pstdin.phi0);/* tangential point での太陽方向を求める */
		std::cout << "sza:" << pstdin.sza << " phi0:" << pstdin.phi0 << std::endl;
	
		sensor_direction(himawari, crosspt.r(), &pstdin.umu, &pstdin.phi);/* crosspt からみた衛星方向を元に、視線方向の局所鉛直からの角（オフナディア角）を求める */
		sensor_theta = acos(pstdin.umu) * Rad2deg;
		std::cout << "sonsor_direction:\n\tsensor_theta:" << sensor_theta <<"\n\tphi:" << pstdin.phi << std::endl;
		std::cout << "cos(sensor_theta) = umu = " << pstdin.umu << std::endl;
	
		save_stdin(path_stdin, pstdin);/* 座標情報を入力ファイルにセーブ */
	
	
		/* TODO ここで大気プロファイル決定、ループ開始 */
	
	
		execute_uvspec(DIR_INTERFACE+path_stdin, DIR_INTERFACE+path_stdout);
	
		if(pstdin.solver == "mystic"){
			radiance[i] = read_mystic_rad(49);
		}else{
			radiance[i] = read_stdout(path_stdout, 0);
		}
	}


	/* fitting and save */
	save_result(path_result, Nheights, heights, obsds[obs_index], radiance);

	
	delete[] radiance;
	return 0;
}

