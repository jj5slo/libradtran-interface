#include <iostream>
#include <chrono>
#include <filesystem>

#include "solar_direction.h"

#include "coordinate.h"
#include "get_msis.h"
#include "interface.h"
#include "read_config.h"
#include "fit.h"
#include "Observed.h"
#include "wrapper.h"

char input;
	
int main(int argc, char *argv[]){
/* ==== 引数処理 ==== */
	int YEAR;
	int MONTH;
	int DAY;
	int HOUR_START;
	int HOUR_END;
	int obs_index;
//	if(argc == 5){
//		YEAR = atoi(argv[1]);
//		MONTH = atoi(argv[2]);
//		DAY = atoi(argv[3]);
//		HOUR_START = 0;
//		HOUR_END = 23;
//		obs_index = atoi(argv[4]) - 1;/* 観測データの何行目を読むか */
//	}
//	else
if(argc == 6){
		YEAR = atoi(argv[1]);
		MONTH = atoi(argv[2]);
		DAY = atoi(argv[3]);
		HOUR_START = atoi(argv[4]);
		HOUR_END = HOUR_START;
		obs_index = atoi(argv[5]) - 1;/* 観測データの何行目を読むか */
	}
	else{
		std::cerr << "Usage: ./main YEAR MONTH DAY OBS_INDEX\nUsage: ./main YEAR MONTH DAY HOUR OBS_INDEX" << std::endl;
		return 0;
	}

/* ==== */

	obsDateTime dt(YEAR, MONTH, DAY, HOUR_START, 0, 0);/* 観測日 *//* TODO HOUR */

/* ==== id付け ==== */
	/* 現在時刻（シミュレーション開始時刻）を取得、保持 */
	auto now = std::chrono::system_clock::now();
	auto nowsec = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch()).count();
	std::string secid = std::to_string(nowsec);
	/* secid を結果につけることでパラメータを保存 */

/* ==== */
/* ==== ファイルから設定読込 ==== */

	std::string PATH_CONFIG = "./config.conf";
	std::map<std::string, std::string> configs = readConfigFile(PATH_CONFIG);
	
	const int DEBUG = getConfig(configs, "DEBUG", 0);
	const int FLAG_UNDISPLAY_LOG = getConfig(configs, "FLAG_UNDISPLAY_LOG", 0);
	const std::string DIR_LOG = getConfig(configs, "DIR_LOG", "/tmp/");/* libRadtranの標準エラーを、標準出力に出さないとき(FLAG_UNDISPLAY_LOG=1)にログとして保存するディレクトリ */

	const std::string DIR_INTERFACE = getConfig(configs, "DIR_INTERFACE", std::string(std::getenv("HOME"))+"/SANO/research/estimate-profile/libradtran-interface/");/* このプログラムが置かれるディレクトリ */
	const std::string DIR_UVSPEC = getConfig(configs, "DIR_UVSPEC", std::string(std::getenv("HOME"))+"/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/bin/");/* uvspec(libRadtran本体)の置かれたディレクトリ */

	std::string DIR_RESULT = getConfig(configs, "DIR_RESULT", "../testresult/") + secid;/* 保存先ディレクトリ */
	std::string DIR_OBS = getConfig(configs, "DIR_OBS", "../testdata/");/* 観測データのディレクトリ */
	const std::string PATH_STDIN = getConfig(configs, "PATH_STDIN", "in");/* libRadtran標準入力を一時保存する場所 */
	std::string PATH_STDOUT = getConfig(configs, "PATH_STDOUT", "out");/* libRadtranの標準出力を一時保存する場所 */
	std::string PATH_ATMOSPHERE = getConfig(configs, "PATH_ATMOSPHERE", std::string(std::getenv("HOME"))+"/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/data/atmmod/afglus.dat");/* libRadtranに渡す大気ファイル */
	std::string PATH_ATMOSPHERE_INIT = getConfig(configs, "PATH_ATMOSPHERE_INIT", std::string(std::getenv("HOME"))+"/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/data/atmmod/afglus.dat");/* libRadtranに渡す大気ファイル */
	
	double wavelength = getConfig(configs, "wavelength", 470.0);/* 波長 [nm]. TODO 決まっているので指定方法を変える */
	int i_top = getConfig(configs, "i_top", 64);/* 数密度を求める最高高度（index） */
	int i_bottom = getConfig(configs, "i_bottom", 60);/* 数密度を求める最低高度（index） */
	int N_exp_decay_atm = getConfig(configs, "N_exp_decay_atm", 5);/* 数密度を求める最低高度（index） */

	std::string solver = getConfig(configs, "solver", "mystic");/* libRadtranのソルバ */

	std::string SURFACE_TYPE = getConfig(configs, "SURFACE_TYPE", "ABSORB");/* 表面反射のタイプを指定 */
	double albedo = getConfig(configs, "albedo", 0.3);/* LAMBERT の反射率 */
	double brdf_cam_u10 = getConfig(configs, "brdf_cam_u10", 15.0);/* BRDF_CAM の風速 */
	std::string additional_option = getConfig(configs, "additional_option", "");/* libRadtranの標準入力に追加で書き込む文字列 */
	int mc_photons = getConfig(configs, "mc_photons", 60000);/* MYSTICの回数 デフォルトは300000 */

	int atmosphere_precision = getConfig(configs, "atmosphere_precision", 7);/* MSISから取得する大気の保存時の精度 */

	double XTOL_REL = getConfig(configs, "XTOL_REL", 1.0e-6);/* 最適化終了判定 */
	int FITTING_ADDITION = getConfig(configs, "FITTING_ADDITION", 0);/* フィッティングするために上の層の値を余計に計算する。破棄予定 */
	std::string PATH_OTEHON = getConfig(configs, "PATH_OTEHON", "/lhome/sano2/SANO/research/estimate-profile/Result/Result-12-W5/OTEHON_2022_6_1_3_36.dat");
//	DIR_UVSPEC





/* ==== */
/*
	std::string PATH_ATMOSPHERE = "/home/sano/SANO/research/estimate-profile/atmmod-temporary/afglus-modN.dat";
	PATH_ATMOSPHERE = "/home/sano/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/data/atmmod/afglus.dat";
	PATH_ATMOSPHERE = "/home/sano/SANO/research/estimate-profile/atmmod-temporary/msis-atm.dat";
*/
/* ==== 保存先ディレクトリ作成 ==== */
	std::cout << "create_directory " << DIR_RESULT << std::endl;
	std::filesystem::create_directory(DIR_RESULT);

	if(DEBUG){ std::cin >> input; }

//	std::string path_result = "result.txt";

/* ==== */
	for(int HOUR_i = HOUR_START; HOUR_i <= HOUR_END; HOUR_i++){/* TODO 一時的に変更 */
/* ==== 観測データ読み込み ==== */
	
//		dt.settime(HOUR_i, 0, 0);/* 観測時 */
//		std::string path_obs = obs_path(DIR_OBS, dt);/* 観測日時からデータの名前 */
//		std::cout << path_obs << std::endl;
//		if(DEBUG){ std::cin >> input; }
//		int Nobs = 0;
//		Observed *obsds = read_obs( &Nobs, path_obs );/* 使うのはobsds[obs_index]だけ */
//		std::cout << Nobs << "points" << std::endl;

		int otehon_lines;
		int otehon_columns;
		std::string otehon_header;
		double** otehon = fit::read_result(PATH_OTEHON, otehon_header, otehon_lines, otehon_columns);
		std::cout << "Read Otehon." << std::endl;
		Observed *obsds = new Observed[obs_index + 1];
		obsds[obs_index].set(73.0, 82.0, otehon_lines, otehon[0], otehon[4]);/* TODO TODO TODO HARD CODING !!! */
		
//		for(int i=0; i<otehon_lines; i++){
//			std::cout << otehon[0][i] <<" "<< obsds[obs_index].Data()[i] << std::endl;
//		}


/* ==== */
/* 諸定数の準備 */
		WrapperArgs args;/* declared in wrapper.h */
		args.TOA_height           = obsds[obs_index].maxHeight();
		
		std::filesystem::remove(DIR_LOG+"libRadtran.log");/* ログ容量溢れ防止 */

		double *heights = obsds[obs_index].Heights();
		int Nheights = obsds[obs_index].Nheights();
		std::cout << "lat" << obsds[obs_index].Latitude() << " "  << "lon" << obsds[obs_index].Longitude() << " " << obsds[obs_index].Nheights() << "heights max:" << args.TOA_height << std::endl;
//		for(int i=0; i<obsds[obs_index].Nheights(); i++){
//			std::cout << heights[i] << " " << obsds[obs_index].Data(heights[i]) << "\n";
//		}
		std::cout << std::endl;
	
		if(DEBUG){ std::cin >> input; }
/* ==== 地球、衛星の設定 ==== */
	
		auto earth = PlanetParam( 6370.e3 );
		auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
		/* TODO configに入れる */
	
/* ==== */


		args.pStdin.mc_photons = mc_photons;/* default is 300000 */
		args.pStdin.solver = solver;
		args.pStdin.additional = additional_option;//\npseudospherical";
	/*
		std::cout << &args.pStdin << " " << &args.pStdin.sza << std::endl;
		return 0;
	*/
		
		args.pStdin.atmosphere_file = PATH_ATMOSPHERE;
		
		args.pStdin.SURFACE_TYPE = SURFACE_TYPE;
		args.pStdin.brdf_cam_u10 = brdf_cam_u10;
		args.pStdin.albedo = albedo;/* 地球平均は0.3 */

		args.pStdin.wavelength = wavelength;
		
		Geocoordinate on_ground(earth, himawari, obsds[obs_index].Latitude(), obsds[obs_index].Longitude(), 0.0);/* 観測データにある緯度経度の高度0km 地点のGeocoordinate */
		double ld_alpha = on_ground.alpha()*Rad2deg;
		double sza_on_ground;
		double phi0_on_ground;
		AndoLab::solar_direction(on_ground.latitude(), on_ground.longitude(), dt.DOY(), dt.Hour(), &sza_on_ground, &phi0_on_ground);/* on_ground での太陽方向 */
		
		std::cout << "ld_alpha : " <<  ld_alpha << std::endl;
		std::cout << "sza_on_ground : " <<  sza_on_ground << std::endl;
		std::cout << "phi0_on_ground : " <<  phi0_on_ground << std::endl;

/* ==== MSIS ==== */

		Geocoordinate *tparr = new Geocoordinate[Nheights];
		LookingDirection ld;
		for(int i=0; i<Nheights; i++){
			ld.set( ld_alpha, heights[i]/m2km );/* 見る場所決め */
			tparr[i] = ld.tangential_point( earth, himawari);/* tangential point の配列 */
		}
//		ParamAtmosphere *pAtm = get_msis(dt, tparr, Nheights);/* tangential point でのMSIS大気から求めたパラメタを取得 */
		ParamAtmosphere* pAtm = readParamAtmosphere(PATH_ATMOSPHERE_INIT, Nheights);
		std::cout << "# Atmosphere\nz Nair p T" << std::endl;
		for(int i=0; i<Nheights; i++){
			std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
		}
		std::cout << "# Init\nz Nair p T" << std::endl;
		for(int i=0; i<Nheights; i++){
			std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
		}
		for(int i=i_bottom; i<=i_top; i++){/* TODO NOW MSISから考えている高度範囲だけはずらした上で最適化で戻るかどうか */
//			double supercoef = 1.0;
//			double sigma_z = (pAtm[i_top].z - pAtm[i_bottom].z) / 3.0;
//			supercoef = 2 * std::exp( -(pAtm[i].z - pAtm[i_bottom].z)*(pAtm[i].z - pAtm[i_bottom].z) / 2.0 / sigma_z/sigma_z );
			double super_inv_10_scaleheight = 0.1;
			pAtm[i].Nair = pAtm[i_top+1].Nair * std::pow(10.0, -super_inv_10_scaleheight * (pAtm[i].z - pAtm[i_top+1].z));
			pAtm[i].set_p_from_Nair_T();
		}
		std::cout << "# Modified Atmosphere\nz Nair p T" << std::endl;
		for(int i=0; i<Nheights; i++){
			std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
		}
		
		saveParamAtmosphere(PATH_ATMOSPHERE, pAtm, Nheights, atmosphere_precision);
	}
	return 0;
}
