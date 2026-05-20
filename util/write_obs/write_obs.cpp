#include <iostream>
#include <string>

#include "readwrite.h"
#include "Observed.h"
#include "obsDateTime.h"

std::string DIR_OBS = "/lhome/sano2/DATA/testdata/";
int HIMAWARI_BAND = 1;

int main(int argc, char *argv[]){
/* ==== 引数処理 ==== */
	int YEAR;
	int MONTH;
	int DAY;
	int HOUR;
	int MINUTE;
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
if(argc == 7){
		YEAR      = atoi(argv[1]);
		MONTH     = atoi(argv[2]);
		DAY       = atoi(argv[3]);
		HOUR      = atoi(argv[4]);
		MINUTE    = atoi(argv[5]);
		obs_index = atoi(argv[6]) - 1;/* 観測データの何行目を読むか */
	}
	else{
		std::cerr << "Usage: ./main YEAR MONTH DAY HOUR MINUTE OBS_INDEX\n" << std::endl;
		return 0;
	}

/* ==== */

	obsDateTime dt(YEAR, MONTH, DAY, HOUR, MINUTE, 0);/* 観測日 *//* TODO HOUR */

	Observed obsd;

	std::string path_obs = obs_path(DIR_OBS, dt, HIMAWARI_BAND);/* 観測日時からデータの名前 */
	std::cout << path_obs << std::endl;
	obsd = read_obs(path_obs, obs_index);/* 使うのはobsdだけ */
	if (obsd.Nheights() == 0){
		std::cerr << "No Observation Data!" << std::endl;
		return 0;
	}
	
	std::string header = 
		"# longitude: " + std::to_string(obsd.Longitude()) + "\n"
		+ "# latitude: " + std::to_string(obsd.Latitude()) + "\n"
		+ "# date: " + std::to_string(dt.Year())+" "+std::to_string(dt.Month())+" "+std::to_string(dt.Date()) +"\n"
		+ "# time: " + std::to_string(dt.Hour())+":"+std::to_string(dt.Minute())+":"+std::to_string(dt.Second())+" UT, Hour="+std::to_string(dt.HourWithDecimal()) +"\n"
		+ "# height observed\n";
	readwrite::save_data("observed.dat", header, obsd.Nheights(), 2, obsd.Heights_Data());

	return 0;
}

