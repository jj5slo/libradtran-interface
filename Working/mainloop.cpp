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
#include"read_config.h"


constexpr double BOLTZMANN_CONSTANT = 1.380649e-23;
char input;

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
/* ==== ファイルから設定読込 ==== */

	std::string configfile = "./config.conf";
	std::map<std::string, std::string> configs = readConfigFile(configfile);
	
	const int DEBUG = getConfig(configs, "DEBUG", 0);
	const int FLAG_UNDISPLAY_LOG = getConfig(configs, "FLAG_UNDISPLAY_LOG", 0);

	const std::string DIR_INTERFACE = getConfig(configs, "DIR_INTERFACE", std::string(std::getenv("HOME"))+"/SANO/research/estimate-profile/libradtran-interface/");/* このプログラムが置かれるディレクトリ */
	const std::string DIR_UVSPEC = getConfig(configs, "DIR_UVSPEC", std::string(std::getenv("HOME"))+"/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/bin/");/* uvspec(libRadtran本体)の置かれたディレクトリ */

	std::string dir_result = getConfig(configs, "dir_result", "../testresult") + secid;/* 保存先ディレクトリ */
	std::string dir_obs = getConfig(configs, "dir_obs", "../testdata");/* 観測データのディレクトリ */
	std::string path_stdin = getConfig(configs, "path_stdin", "in");/* libRadtran標準入力を一時保存する場所 */
	std::string path_stdout = getConfig(configs, "path_stdout", "out");/* libRadtranの標準出力を一時保存する場所 */
	std::string path_atmosphere = getConfig(configs, "path_atmosphere", "/home/sano/SANO/research/estimate-profile/atmmod-temporary/msis-atm.dat");/* libRadtranに渡す大気ファイル */
	
	double wavelength = getConfig(configs, "wavelength", 470.0);/* 波長 [nm]. TODO 決まっているので指定方法を変える */

	std::string solver = getConfig(configs, "solver", "mystic");/* libRadtranのソルバ */

	std::string SURFACE_TYPE = getConfig(configs, "SURFACE_TYPE", "ABSORB");/* 表面反射のタイプを指定 */
	double albedo = getConfig(configs, "albedo", 0.3);/* LAMBERT の反射率 */
	double brdf_cam_u10 = getConfig(configs, "brdf_cam_u10", 15.0);/* BRDF_CAM の風速 */
	std::string additional_option = getConfig(configs, "additional_option", "");/* libRadtranの標準入力に追加で書き込む文字列 */
	int mc_photons = getConfig(configs, "mc_photons", 60000);/* MYSTICの回数 デフォルトは300000 */

	int atmosphere_precision = getConfig(configs, "atmosphere_precision", 7);/* MSISから取得する大気の保存時の精度 */

//	DIR_UVSPEC





/* ==== */
/*
	std::string path_atmosphere = "/home/sano/SANO/research/estimate-profile/atmmod-temporary/afglus-modN.dat";
	path_atmosphere = "/home/sano/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/data/atmmod/afglus.dat";
	path_atmosphere = "/home/sano/SANO/research/estimate-profile/atmmod-temporary/msis-atm.dat";
*/
/* ==== 保存先ディレクトリ作成 ==== */
	std::cerr << dir_result << std::endl;
	std::filesystem::create_directory(dir_result);

	if(DEBUG){ std::cin >> input; }

//	std::string path_result = "result.txt";

/* ==== */
/* ==== 観測データ読み込み ==== */

	for(int HOUR = 0; HOUR < 24; HOUR++){
		dt.settime(HOUR, 0, 0);/* 観測時 */
		std::string path_obs = obs_path(dir_obs, dt);/* 観測日時からデータの名前 */
		std::cerr << path_obs << std::endl;
		if(DEBUG){ std::cin >> input; }
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
	
		if(DEBUG){ std::cin >> input; }
/* ==== 地球、衛星の設定 ==== */
	
		auto earth = PlanetParam( 6370.e3 );
		auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
	
/* ==== */

		int doy = dt.DOY();/*  */
		ParamStdin pstdin;
		pstdin.mc_photons = mc_photons;/* default is 300000 */
		pstdin.solver = solver;
		pstdin.additional = additional_option;//\npseudospherical";
	/*
		std::cout << &pstdin << " " << &pstdin.sza << std::endl;
		return 0;
	*/
		
		pstdin.atmosphere_file = path_atmosphere;
		
		pstdin.SURFACE_TYPE = SURFACE_TYPE;
		pstdin.brdf_cam_u10 = brdf_cam_u10;
		pstdin.albedo = albedo;/* 地球平均は0.3 */

		double sensor_theta;
	
		pstdin.wavelength = wavelength;
		
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
		saveParamAtmosphere(path_atmosphere, pAtom, Nheights, atmosphere_precision);

/* ==== */
/* MSISで求めた大気をNLoptの初期値に代入する。最小化する評価関数はwrapperとして実装するが、
*/

		double *radiance = new double [Nheights];/* シミュレーション結果 */
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
		
			if(DEBUG){ std::cin >> input; }
		
			/* TODO ここで大気プロファイル決定、ループ開始 */
			
			/* delete_mystic_rad(); */
		
			execute_uvspec(DIR_UVSPEC, DIR_INTERFACE+path_stdin, DIR_INTERFACE+path_stdout, FLAG_UNDISPLAY_LOG);
		
			if(pstdin.solver == "mystic"){
				radiance[i] = read_mystic_rad(DIR_UVSPEC, 105);/* TODO この層番号の決め方がいまいちわからない 0から100km, 1kmごとであればTOAで105 */
			}else{
				radiance[i] = read_stdout(path_stdout, 0);
			}
			std::cerr << "Radiance: " << radiance[i] << "\n----" << std::endl;
			if(DEBUG){ std::cin >> input; }
		}	
		/* save */
		std::string path_result = save_path(dir_result, secid, dt, ld_alpha);
		save_result(path_result, secid, on_ground, Nheights, heights, obsds[obs_index], radiance);
		save_params(dir_result, secid, path_atmosphere, "_atm"+std::to_string(HOUR)+".txt");/* atmosphereも保存しておく */
	}	
	/* 最後だけパラメータ保存 */
	save_params(dir_result, secid, path_stdin, "_stdin.txt");
	save_params(dir_result, secid, configfile, "_config.conf");

//	delete[] radiance;
	return 0;
}

