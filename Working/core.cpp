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

#include "wrapper.h"

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
		if(DEBUG){ std::cin >> input; }
		int Nobs = 0;
		Observed *obsds = read_obs( &Nobs, path_obs );/* 使うのはobsds[obs_index]だけ */
		std::cout << Nobs << "points" << std::endl;
		
		std::ofstream ofs("./hydrostat/"+std::to_string(dt.Year())+"-"+std::to_string(dt.Month())+"-"+std::to_string(dt.Date())+"-"+std::to_string(dt.Hour())+"_"+std::to_string(obs_index)+".dat");
		// "_lat"+std::to_string(obsds[obs_index].Latitude())+"_lon"+std::to_string(obsds[obs_index].Longitude())+".dat");
		ofs << "# " << dt.Year() <<" "<< dt.Month() <<" "<< dt.Date() <<" time:"<< dt.Hour() <<"\n";
		ofs << "# " << path_obs << std::endl;

/* ==== */

		double *heights = obsds[obs_index].Heights();
		int Nheights = obsds[obs_index].Nheights();
		double maxHeight = obsds[obs_index].maxHeight();
		std::cout << "lat" << obsds[obs_index].Latitude() << " "  << "lon" << obsds[obs_index].Longitude() << " " << obsds[obs_index].Nheights() << "heights max:" << maxHeight << std::endl;
		ofs <<"# " << "lat" << obsds[obs_index].Latitude() << " "  << "lon" << obsds[obs_index].Longitude() << " " << obsds[obs_index].Nheights() << "heights max:" << maxHeight << std::endl;
//		for(int i=0; i<obsds[obs_index].Nheights(); i++){
//			std::cout << heights[i] << " " << obsds[obs_index].Data(heights[i]) << "\n";
//		}
		std::cout << std::endl;
	
		if(DEBUG){ std::cin >> input; }
/* ==== 地球、衛星の設定 ==== */
	
		auto earth = PlanetParam( 6370.e3 );/* changed 6370 -> 6371 2025/11/5 15:43 */ /* 6371 -> 6370 2025/11/13 15:01 */
		auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
	
/* ==== */

		int doy = dt.DOY();/*  */
		ParamStdin pStdin;
		pStdin.mc_photons = mc_photons;/* default is 300000 */
		pStdin.solver = solver;
		pStdin.additional = additional_option;//\npseudospherical";
	/*
		std::cout << &pStdin << " " << &pStdin.sza << std::endl;
		return 0;
	*/
		
		pStdin.atmosphere_file = path_atmosphere;
		
		pStdin.SURFACE_TYPE = SURFACE_TYPE;
		pStdin.brdf_cam_u10 = brdf_cam_u10;
		pStdin.albedo = albedo;/* 地球平均は0.3 */

		pStdin.wavelength = wavelength;
		
		Geocoordinate on_ground(earth, himawari, obsds[obs_index].Latitude(), obsds[obs_index].Longitude(), 0.0);/* 観測データにある緯度経度の高度0km 地点のGeocoordinate */
		double ld_alpha = on_ground.alpha()*Rad2deg;
		
		std::cout << "ld_alpha : " <<  ld_alpha << std::endl;
		ofs <<"# " << "ld_alpha : " <<  ld_alpha << std::endl;

/* ==== MSIS ==== */

		Geocoordinate *tparr = new Geocoordinate[Nheights];
		LookingDirection ld;
		for(int i=0; i<Nheights; i++){
			ld.set( ld_alpha, heights[i]/m2km );/* 見る場所決め */
			tparr[i] = ld.tangential_point( earth, himawari);/* tangential point の配列 */
		}
		ParamAtmosphere *pAtm = get_msis(dt, tparr, Nheights);/* tangential point でのMSIS大気から求めたパラメタを取得 */
//		saveParamAtmosphere(path_atmosphere, pAtm, Nheights, atmosphere_precision);
		std::vector<double> x(Nheights, 0.0);/* 要素数指定、初期化 */
		for(int i=0; i<Nheights; i++){
			x[i] = pAtm[i].Nair;/* [cm-3] */
		}

//		double* gm_e = msis_to_gm_e(pAtm, earth, dt, tparr, Nheights);
//		for(int i=0; i<Nheights-1; i++){
//			std::cout << i <<" "<< gm_e[i] <<"\n";
//		}
		ParamAtmosphere *newatm = Nair_to_atmosphere(Nheights, dt, tparr, earth, x, pAtm[Nheights-1].p);

		std::cout << "# atm_msis|newatm\nz Nair p T | z Nair p T | Dp DT" << std::endl;
		ofs << "# atm_msis|newatm\nz Nair p T | z Nair p T | Dp DT" << std::endl;

		for(int i=0; i<Nheights; i++){
			std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << " " << newatm[i].z<< " " << newatm[i].Nair<< " " << newatm[i].p<< " " << newatm[i].T << " " << pAtm[i].p-newatm[i].p <<" "<< pAtm[i].T-newatm[i].T << std::endl;
			ofs << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << " " << newatm[i].z<< " " << newatm[i].Nair<< " " << newatm[i].p<< " " << newatm[i].T << " " << pAtm[i].p-newatm[i].p <<" "<< pAtm[i].T-newatm[i].T << std::endl;
		}
		ofs.close();
		continue;
	}
	return 0;
/* ==== */
/* MSISで求めた大気をNLoptの初期値に代入する。最小化する評価関数はwrapperとして実装するが、
*/

//		double *radiances = new double [Nheights];/* シミュレーション結果 */
//		for(int i=0; i<Nheights; i++){
//			set_stdin_direction( i, pStdin, tparr, earth, himawari, maxHeight, doy, dt );
//			save_stdin(path_stdin, pStdin);/* 座標情報を入力ファイルにセーブ */
//			radiances[i] = acquire_radiance(DIR_UVSPEC, PATH_STDIN, PATH_STDOUT, FLAG_UNDISPLAY_LOG, pStdin.solver);
//		}	
//		/* save */
//		std::string path_result = save_path(dir_result, secid, dt, ld_alpha);
//		save_result(path_result, secid, on_ground, Nheights, heights, obsds[obs_index], radiances);
//		save_params(dir_result, secid, path_atmosphere, "_atm"+std::to_string(HOUR)+".txt");/* atmosphereも保存しておく */
//	}	
//	/* 最後だけパラメータ保存 */
//	save_params(dir_result, secid, path_stdin, "_stdin.txt");
//	save_params(dir_result, secid, configfile, "_config.conf");
//
////	delete[] radiances;
//	return 0;

}

