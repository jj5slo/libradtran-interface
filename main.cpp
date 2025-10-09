#include<iostream>
#include<cstdlib>
#include<algorithm>/* 最大最小用 */
#include<chrono>
#include<filesystem>

#include"solar_direction.h"

#include"coordinate.h"
#include"interface.h"
#include"execute.h"
#include"save.h"
#include"get_msis.h"

constexpr double BOLTZMANN_CONSTANT = 1.380649e-23;

int main(int argc, char *argv[]){
/* ==== 引数処理 ==== */

	if(argc != 5){
		std::cerr << "Usage: ./main YEAR MONTH DAY OBS_INDEX\n";
		return 0;
	}
	const int YEAR = atoi(argv[1]);
	const int MONTH = atoi(argv[2]);
	const int DAY = atoi(argv[3]);
	const int obs_index = atoi(argv[4]);/* 観測データの何行目を読むか */

/* ==== */

	obsDateTime dt(YEAR, MONTH, DAY);/* 観測日 */

/* ==== id付け ==== */
/* 現在時刻（シミュレーション開始時刻）を取得、保持 */
	auto now = std::chrono::system_clock::now();
	auto nowsec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
	std::string secid = std::to_string(nowsec);
	/* secid を結果につけることでパラメータを保存 */

/* ==== */
/* ==== 保存先ディレクトリ等指定 ==== */

	std::string dir_result = "../testresult/" + secid;/* ここに保存 */
	std::filesystem::create_directory(dir_result);

	std::string path_stdin = "in";
	std::string path_stdout = "out";
//	std::string path_result = "result.txt";
	/* 観測データ読み込み */
	std::string path_atmosphere = "/home/sano/SANO/research/estimate-profile/atmmod-temporary/afglus-modN.dat";
	path_atmosphere = "/home/sano/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/data/atmmod/afglus.dat";

	path_atmosphere = "/home/sano/SANO/research/estimate-profile/atmmod-temporary/msis-atm.dat";

/* ==== */
/* ==== 観測データ読み込み ==== */

	for(int HOUR = 0; HOUR < 24; HOUR++){
		dt.settime(HOUR, 0, 0);/* 観測時 */
		std::string path_obs = obs_path("../testdata/", dt);/* 観測日時からデータの名前 */
		std::cerr << path_obs << std::endl;
		int Nobs = 0;
		Observed *obsds = read_obs( &Nobs, path_obs );/* 使うのはobsds[obs_index]だけ */
		std::cout << Nobs << "points" << std::endl;

/* ==== */

		double *heights = obsds[obs_index].Heights();
		int Nheights = obsds[obs_index].Nheights();
		double maxHeight = obsds[obs_index].maxHeight();
		std::cout << "lat" << obsds[obs_index].Latitude() << " "  << "lon" << obsds[obs_index].Longitude() << " " << obsds[obs_index].Nheights() << "heights max:" << maxHeight << std::endl;
		for(int i=0; i<obsds[obs_index].Nheights(); i++){
			std::cout << heights[i] << " " << obsds[obs_index].Data(heights[i]) << "\n";
		}
		std::cout << std::endl;
	
/* ==== 地球、衛星の設定 ==== */
	
		auto earth = PlanetParam( 6370.e3 );
		auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
	
/* ==== */

		int doy = dt.DOY();/*  */
		ParamStdin pstdin;
		pstdin.mc_photons = 60000;/* default is 300000 */
		pstdin.solver = "mystic";
		pstdin.additional = "output_user uu";//\npseudospherical";
	/*
		std::cout << &pstdin << " " << &pstdin.sza << std::endl;
		return 0;
	*/

		pstdin.atmosphere_file = path_atmosphere;
		pstdin.brdf_cam_u10 = 15;

		double sensor_theta;
		double *radiance = new double [Nheights];		
	
		pstdin.wavelength = 470.0;
//		pstdin.albedo = 0.3;/* 地球平均 */
		
		Geocoordinate on_ground(earth, himawari, obsds[obs_index].Latitude(), obsds[obs_index].Longitude(), 0.0);/* 観測データにある緯度経度の高度0km 地点のGeocoordinate */
		double ld_alpha = on_ground.alpha()*Rad2deg;
		
		std::cout << "ld_alpha : " <<  ld_alpha << std::endl;

/* ==== MSIS ==== */

		Geocoordinate *tparr = new Geocoordinate[Nheights];
		LookingDirection ld;
		for(int i=0; i<Nheights; i++){
			ld.set( ld_alpha, heights[i]/m2km );/* 見る場所決め */
			tparr[i] = ld.tangential_point( earth, himawari);/* tangential point の配列 */
		}
		ParamAtmosphere *pAtom = get_msis(dt, tparr, Nheights);/* tangential point でのMSIS大気から求めたパラメタを取得 */
		saveParamAtmosphere(path_atmosphere, pAtom, Nheights, 7);

/* ==== */

	for(int i=0; i<Nheights; i++){
			Geocoordinate tp = tparr[i];
			std::cout << "Tangential point:\n";
			std::cout << "\tlat:" << tp.latitude() << "\n\tlon:" << tp.longitude() << "\nt\taltitude:" << tp.altitude() << "\n\talpha:" << tp.alpha()*Rad2deg << std::endl;


			AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
			crosspts = Across_point_atmosphere(earth, himawari, tp.r(), maxHeight); /* maxHeight=TOA */
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
				radiance[i] = read_mystic_rad(105);/* TODO この層番号の決め方がいまいちわからない */
			}else{
				radiance[i] = read_stdout(path_stdout, 0);
			}
		}	
		/* save */
		std::string path_result = save_path(dir_result, secid, dt, ld_alpha);
		save_result(path_result, secid, on_ground, Nheights, heights, obsds[obs_index], radiance);
	}	
	/* 最後だけパラメータ保存 */
	save_params(dir_result, secid, path_stdin, "_stdin.txt");
	save_params(dir_result, secid, path_atmosphere, "_atm.txt");/* atmosphereも保存しておく */

//	delete[] radiance;
	return 0;
}

