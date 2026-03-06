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
	int HOUR;
	int MINUTE;
	int OBS_INDEX;
//	if(argc == 5){
//		YEAR = atoi(argv[1]);
//		MONTH = atoi(argv[2]);
//		DAY = atoi(argv[3]);
//		HOUR_START = 0;
//		HOUR_END = 23;
//		obs_index = atoi(argv[4]) - 1;/* 観測データの何行目を読むか */
//	}
//	else

	int atmosphere_precision = 7;/* MSISから取得する大気の保存時の精度 */
	int FLAG_OBS    = 0;
	int FLAG_OUTPUT = 0;
	std::string DIR_OBS = "";
	std::string PATH_OUT = "";
	
	int option_number = 0;
	for(int i=1; i<argc; i++){
		if(std::string(argv[i]) == "-h"){
			std::cerr << "Usage: ./save_msis_atm YEAR MONTH DAY HOUR MINUTE OBS_INDEX -i DIR_OBS [-o OUTPUT] [-p PRECISION]\n" << std::endl;
			return 0;
		}
		else if(std::string(argv[i]) == "-i"){
			FLAG_OBS = 1;
			DIR_OBS = std::string(argv[++i]);
		}
		else if(std::string(argv[i]) == "-o"){
			FLAG_OUTPUT = 1;
			PATH_OUT = std::string(argv[++i]);
		}
		else{
			switch (option_number){
				case 0:
					YEAR   = atoi(argv[i]);
					break;
				case 1:
					MONTH  = atoi(argv[i]); 
					break;
				case 2:
					DAY    = atoi(argv[i]);
					break;
				case 3:
					HOUR   = atoi(argv[i]);
					break;
				case 4:
					MINUTE = atoi(argv[i]);
					break;
				case 5:
					OBS_INDEX = atoi(argv[i]) - 1;
					break;
				default:
					std::cerr << "too much arguments.\nUsage: ./save_msis_atm YEAR MONTH DAY HOUR MINUTE OBS_INDEX -i DIR_OBS [-o OUTPUT] [-p PRECISION]\n" << std::endl;
					return 0;
			}
			++option_number;
		}
	}
	if(FLAG_OBS != 1){
		std::cerr << "-i needed.\nUsage: ./save_msis_atm YEAR MONTH DAY HOUR MINUTE OBS_INDEX -i DIR_OBS [-o OUTPUT] [-p PRECISION]\n" << std::endl;
		return 0;
	}
	if(option_number != 6){
		std::cerr << "too less arguments.\nUsage: ./save_msis_atm YEAR MONTH DAY HOUR MINUTE OBS_INDEX -i DIR_OBS [-o OUTPUT] [-p PRECISION]\n" << std::endl;
		return 0;
	}
					
/* ==== */

	obsDateTime dt(YEAR, MONTH, DAY, HOUR, MINUTE, 0);/* 観測日 */

/* ==== 保存先ディレクトリ作成 ==== */

//	std::string path_result = "result.txt";

/* ==== */
/* ==== 観測データ読み込み ==== */
	std::string path_obs = obs_path(DIR_OBS, dt);/* 観測日時からデータの名前 */
	std::cout << path_obs << std::endl;
	int Nobs = 0;
	Observed *obsds = read_obs( &Nobs, path_obs );/* 使うのはobsds[OBS_INDEX]だけ */
	std::cout << Nobs << "points" << std::endl;
/* ==== */
/* 諸定数の準備 */
	
	double *heights = obsds[OBS_INDEX].Heights();
	int Nheights = obsds[OBS_INDEX].Nheights();
	std::cout << "lat" << obsds[OBS_INDEX].Latitude() << " "  << "lon" << obsds[OBS_INDEX].Longitude() << " " << obsds[OBS_INDEX].Nheights() << "heights max:" << obsds[OBS_INDEX].maxHeight() << std::endl;
//		for(int i=0; i<obsds[OBS_INDEX].Nheights(); i++){
//			std::cout << heights[i] << " " << obsds[OBS_INDEX].Data(heights[i]) << "\n";
//		}
	std::cout << std::endl;

/* ==== 地球、衛星の設定 ==== */
	
	auto earth = PlanetParam( 6370.e3 );
	auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
	/* TODO configに入れる */

/* ==== */


/*
	std::cout << &args.pStdin << " " << &args.pStdin.sza << std::endl;
	return 0;
*/
	
	
	Geocoordinate on_ground(earth, himawari, obsds[OBS_INDEX].Latitude(), obsds[OBS_INDEX].Longitude(), 0.0);/* 観測データにある緯度経度の高度0km 地点のGeocoordinate */
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
	ParamAtmosphere *pAtm = get_msis(dt, tparr, Nheights);/* tangential point でのMSIS大気から求めたパラメタを取得 */
	std::cout << "# MSIS\nz Nair p T" << std::endl;
	for(int i=0; i<Nheights; i++){
		std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
	}
//		for(int i=i_bottom; i<=i_top; i++){/* TODO NOW MSISから考えている高度範囲だけはずらした上で最適化で戻るかどうか */
////			double supercoef = 1.0;
////			double sigma_z = (pAtm[i_top].z - pAtm[i_bottom].z) / 3.0;
////			supercoef = 2 * std::exp( -(pAtm[i].z - pAtm[i_bottom].z)*(pAtm[i].z - pAtm[i_bottom].z) / 2.0 / sigma_z/sigma_z );
//			double super_inv_10_scaleheight = 0.1;
//			pAtm[i].Nair = pAtm[i_top+1].Nair * std::pow(10.0, -super_inv_10_scaleheight * (pAtm[i].z - pAtm[i_top+1].z));
//			pAtm[i].set_p_from_Nair_T();
//		}
//	std::cout << "# Modified Atmosphere\nz Nair p T" << std::endl;
//	for(int i=0; i<Nheights; i++){
//		std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
//	}
	if(FLAG_OUTPUT){	
		saveParamAtmosphere(PATH_OUT, pAtm, Nheights, atmosphere_precision);
	}
	return 0;
}
