#include<iostream>
#include<cstdlib>
#include<algorithm>/* 最大最小とか用 */
#include<chrono>
#include<filesystem>

#include"solar_direction.h"

#include"coordinate.h"
#include"interface.h"
#include"execute.h"
#include"save.h"

constexpr int NLINES = 3;
constexpr double BOLTZMANN_CONSTANT = 1.380649e-23;

int main(int argc, char *argv[]){
	
	if(argc != 5){
		std::cerr << "Usage: ./main YEAR MONTH DAY OBS_INDEX\n";
		return 0;
	}
	const int YEAR = atoi(argv[1]);
	const int MONTH = atoi(argv[2]);
	const int DAY = atoi(argv[3]);
	const int obs_index = atoi(argv[4]);/* 観測データの何行目を読むか */
	
	obsDateTime dt(YEAR, MONTH, DAY);

	/* 現在時刻（シミュレーション開始時刻）を取得、保持 */
	auto now = std::chrono::system_clock::now();
	auto nowsec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
	std::string secid = std::to_string(nowsec);
	/* secid を結果につけることでパラメータを保存 */

	std::string dir_result = "../testresult/" + secid;/* ここに保存 */
	std::filesystem::create_directory(dir_result);

	std::string path_stdin = "in";
	std::string path_stdout = "out";
//	std::string path_result = "result.txt";
	/* 観測データ読み込み */
	for(int HOUR = 0; HOUR < 24; HOUR++){
		dt.settime(HOUR, 0, 0);
		std::string path_obs = obs_path("../testdata/", dt);//"../h08_b01_s01s02_20220501_171000.txt";
		std::cerr << path_obs << std::endl;
		int Nobs = 0;
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
	
		int doy = dt.DOY();/*  */
		/*
		double *sza = new double;
		double *phi0 = new double;
		double *mu = new double;
		double *phi = new double;
		*/
		ParamStdin pstdin;
		pstdin.mc_photons = 60000;/* default is 300000 */
		pstdin.solver = "mystic";
		pstdin.additional = "output_user uu";//\npseudospherical";
	/*
		std::cout << &pstdin << " " << &pstdin.sza << std::endl;
		return 0;
	*/
		std::string path_atmosphere = "/home/sano/SANO/research/estimate-profile/atmmod-temporary/afglus-modN.dat";
		path_atmosphere = "/home/sano/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/data/atmmod/afglus.dat";

		pstdin.atmosphere_file = path_atmosphere;
		pstdin.brdf_cam_u10 = 15;

		double sensor_theta;
		double *radiance = new double [Nheights];		
	
		pstdin.wavelength = 470.0;
		pstdin.albedo = 0.3;/* 地球平均 */
		
		Geocoordinate on_ground(earth, himawari, obsds[obs_index].Latitude(), obsds[obs_index].Longitude(), 0.0);/* 観測データにある緯度経度の高度0km 地点のGeocoordinate */
		double ld_alpha = on_ground.alpha()*Rad2deg;
		
		std::cout << "ld_alpha : " <<  ld_alpha << std::endl;
	
	for(int i=0; i<Nheights; i++){
			LookingDirection ld( ld_alpha, heights[i]/m2km );/* 見る場所決め */
			Geocoordinate tp = ld.tangential_point( earth, himawari );/* 見る場所が実際どの座標なのか？ */
			std::cout << "Tangential point:\n";
			std::cout << "\tlat:" << tp.latitude() << "\n\tlon:" << tp.longitude() << "\nt\taltitude:" << tp.altitude() << "\n\talpha:" << tp.alpha()*Rad2deg << std::endl;
			
//			/* TEST */
//			int NpAtom = 11;
//			ParamAtmosphere *pAtom = new ParamAtmosphere [NpAtom];/* 10分割した大気 */
//			pAtom[0].z = 0.0;
//			pAtom[0].T = 300.0;
//			pAtom[0].Nair = 2.545818e+19;
//			pAtom[1].z = 12.0;
//			pAtom[1].T = 300.0;
//			pAtom[1].Nair = 2.545818e+18;
//			pAtom[2].z = 24.0;
//			pAtom[2].T = 300.0;
//			pAtom[2].Nair = 2.545818e+17;
//			pAtom[3].z = 36.0;
//			pAtom[3].T = 300.0;
//			pAtom[3].Nair = 2.545818e+16;
//			pAtom[4].z = 48.0;
//			pAtom[4].T = 310.0;
//			pAtom[4].Nair = 2.545818e+15;
//			pAtom[5].z = 60.0;
//			pAtom[5].T = 320.0;
//			pAtom[5].Nair = 2.545818e+14;
//			pAtom[6].z = 72.0;
//			pAtom[6].T = 330.0;
//			pAtom[6].Nair = 2.545818e+13;
//			pAtom[7].z = 84.0;
//			pAtom[7].T = 340.0;
//			pAtom[7].Nair = 2.545818e+11;
//			pAtom[8].z = 96.0;
//			pAtom[8].T = 350.0;
//			pAtom[8].Nair = 2.545818e+11;
//			pAtom[9].z = 108.0;
//			pAtom[9].T = 360.0;
//			pAtom[9].Nair = 2.545818e+19;
//			pAtom[10].z = 120.0;
//			pAtom[10].T = 600.0;
//			pAtom[10].Nair = 1.0e+3;
//			for(int i = 0; i<NpAtom; i++){
//				pAtom[i].p = pAtom[i].Nair * 100*100 /* *100/100 */ * BOLTZMANN_CONSTANT * pAtom[i].T;
//				std::cout << pAtom[i].p << std::endl;
//			}
////			return 0;
//			
//			/* なんの意味もない数字 */
//			saveParamAtmosphere(path_atmosphere, pAtom, NpAtom, 7);
////			return 0;

			AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
			crosspts = Across_point_atmosphere(earth, himawari, tp.r());
			Geocoordinate crosspt(earth, himawari, crosspts[0]);/* TOAと視線の交点を求める */
			std::cout << "crosspoint_of_atmosphere:\n\tlat:" << crosspt.latitude() << "\n\tlon:" << crosspt.longitude() << "\n\taltitude:" << crosspt.altitude() << std::endl;
			
			AndoLab::solar_direction(crosspt.latitude(), crosspt.longitude(), doy, dt.Hour(), &pstdin.sza, &pstdin.phi0);/* tangential point での太陽方向を求める */
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
		std::string path_result = save_path(dir_result, secid, dt, ld_alpha);
		save_result(path_result, secid, on_ground, Nheights, heights, obsds[obs_index], radiance);
	}	
	/* 最後だけパラメータ保存 */
	save_params(dir_result, secid, path_stdin);
	
//	delete[] radiance;
	return 0;
}

