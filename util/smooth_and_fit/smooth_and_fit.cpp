#include <iostream>
#include <cstdlib>
#include <algorithm>/* 最大最小用 */
#include <chrono>
#include <filesystem>

#include "solar_direction.h"

#include "coordinate.h"
#include "interface.h"
#include "execute.h"
#include "save.h"
#include "get_msis.h"
#include "read_config.h"
#include "wrapper.h"

char input;

/* ==== main ==== */
/* 
 *
 *
 *
 */
/* ==== */


double core(void* raw_Args);
int main(int argc, char *argv[]){
/* ==== 引数処理 ==== */
	int YEAR;
	int MONTH;
	int DAY;
	int HOUR_START;
	int HOUR_END;
	int obs_index;
	std::string PATH_RAW;
	int COL_RAW;
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
		PATH_RAW = argv[6];
		COL_RAW = atoi(argv[7]) - 1;
	}
	else{
		std::cerr << "Usage: ./smooth_and_fit YEAR MONTH DAY HOUR OBS_INDEX PATH_RAW COL_RAW" << std::endl;
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

	std::string DIR_RESULT = getConfig(configs, "DIR_RESULT", "../testresult/") + secid;/* 保存先ディレクトリ */
	std::string DIR_OBS = getConfig(configs, "DIR_OBS", "../testdata/");/* 観測データのディレクトリ */
	
	int i_top = getConfig(configs, "i_top", 64);/* 数密度を求める最高高度（index） */
	int i_bottom = getConfig(configs, "i_bottom", 60);/* 数密度を求める最低高度（index） */

	double offset_bottom_height = getConfig(configs, "offset_bottom_height", 89.9);/* for fit */
	double offset_top_height    = getConfig(configs, "offset_top_height", 100.1);/* for fit */

	int    N_running_mean       = getConfig(configs, "N_running_mean", 3);



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
	
		dt.settime(HOUR_i, 0, 0);/* 観測時 */
		std::string path_obs = obs_path(DIR_OBS, dt);/* 観測日時からデータの名前 */
		std::cout << path_obs << std::endl;
		if(DEBUG){ std::cin >> input; }
		int Nobs = 0;
		Observed *obsds = read_obs( &Nobs, path_obs );/* 使うのはobsds[obs_index]だけ */
		std::cout << Nobs << "points" << std::endl;
		Observed obs = obsds[obs_index];
/* ==== */
	
		std::string header;
		int Nlines;
		int Ncolumns;
		double** raw_raw_data = fit::read_result(PATH_RAW, header, Nlines, Ncolumns);
		double* raw_data = raw_raw_data[COL_RAW];
		double* smoothed = fit::running_mean_log(Nlines, N_running_mean, raw_data);
		double offset = fit::mean(obs.Nheights(), obs.Heights(), offset_bottom_height, offset_top_height);
		double* a_offset = fit::obtain_fitting_coefficient(obs.Data(), smoothed, i_bottom, i_top, offset


/* 諸定数の準備 */
/* ==== 地球、衛星の設定 ==== */
	
	
//	delete[] radiance;
	return 0;
}

