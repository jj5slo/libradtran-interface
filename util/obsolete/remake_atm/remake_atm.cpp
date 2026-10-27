#include <iostream>
#include <cstdlib>
#include <algorithm>/* 最大最小用 */
#include <chrono>
#include <filesystem>
#include <memory>

#include "save.h"
#include "get_msis.h"
#include "readwrite.h"

//constexpr int i_top    {64};/* 全体のどこかでは誤差計算に含める */
//constexpr int i_bottom {60};
//constexpr int N_exp_decay_atm {5}; /* この範囲の点数では数密度は指数的に変化するものとする(これごとに分割してもとめる) */
double lat0s[88] = { 38.0,  39.0,  40.0,  41.0,  42.0,  43.0,  44.0,  45.0,  46.0,  47.0,  48.0,  49.0,  50.0,  51.0,  52.0,  53.0,  54.0,  55.0,  56.0,  57.0,  58.0,  59.0,  60.0,  61.0,  62.0,  63.0,  64.0,  65.0,  66.0,  67.0,  68.0,  69.0,  70.0,  71.0,  72.0,  73.0,  74.0,  75.0,  76.0,  77.0,  78.0,  79.0,  80.0,  81.0,  81.0,  80.0,  79.0,  78.0,  77.0,  76.0,  75.0,  74.0,  73.0,  72.0,  71.0,  70.0,  69.0,  68.0,  67.0,  66.0,  65.0,  64.0,  63.0,  62.0,  61.0,  60.0,  59.0,  58.0,  57.0,  56.0,  55.0,  54.0,  53.0,  52.0,  51.0,  50.0,  49.0,  48.0,  47.0,  46.0,  45.0,  44.0,  43.0,  42.0,  41.0,  40.0,  39.0,  38.0};
double lon0s[88] = { 61.8,  62.0,  62.2,  62.3,  62.5,  62.7,  62.9,  63.1,  63.4,  63.6,  63.8,  64.1,  64.4,  64.7,  65.0,  65.3,  65.7,  66.1,  66.5,  66.9,  67.4,  67.9,  68.4,  69.0,  69.6,  70.3,  71.0,  71.8,  72.7,  73.6,  74.6,  75.8,  77.1,  78.5,  80.2,  82.0,  84.2,  86.7,  89.7,  93.3,  97.8, 103.7, 112.3, 128.8, 152.6, 169.1, 177.7, 183.6, 188.1, 191.7, 194.7, 197.2, 199.4, 201.2, 202.9, 204.3, 205.6, 206.8, 207.8, 208.7, 209.6, 210.4, 211.1, 211.8, 212.4, 213.0, 213.5, 214.0, 214.5, 214.9, 215.3, 215.7, 216.1, 216.4, 216.7, 217.0, 217.3, 217.6, 217.8, 218.0, 218.3, 218.5, 218.7, 218.9, 219.1, 219.2, 219.4, 219.6};
constexpr int Nheights = 101;
int atmosphere_precision = 7;

int main(int argc, char *argv[]){
/* ==== 引数処理 ==== */
	int YEAR;
	int MONTH;
	int DAY;
	int HOUR;
	int MINUTE;
	int obs_index;
	std::string PATH;
	int iTOP;
	int HEIGHT_RANGE;
if(argc == 10){
		YEAR      = atoi(argv[1]);
		MONTH     = atoi(argv[2]);
		DAY       = atoi(argv[3]);
		HOUR      = atoi(argv[4]);
		MINUTE    = atoi(argv[5]);
		obs_index = atoi(argv[6]) - 1;/* 観測データの何行目を読むか */
		PATH      = std::string(argv[7]);
		iTOP      = atoi(argv[8]);
		HEIGHT_RANGE = atoi(argv[9]);
	}
	else{
		std::cerr << "Usage: ./main YEAR MONTH DAY HOUR MINUTE OBS_INDEX PATH iTOP HEIGHT_RANGE\n" << std::endl;
		return 0;
	}

/* ==== */

	obsDateTime dt(YEAR, MONTH, DAY, HOUR, MINUTE, 0);/* 観測日 *//* TODO HOUR */
	
	auto earth = PlanetParam( 6370.e3 );
	auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
	
	Geocoordinate on_ground(earth, himawari, lat0s[obs_index], lon0s[obs_index], 0.0);/* 観測データにある緯度経度の高度0km 地点のGeocoordinate */
	double ld_alpha = on_ground.alpha()*Rad2deg;

/* ==== MSIS ==== */
	double* heights = new double[Nheights];
	for(int i=0; i<Nheights; ++i){
		heights[i] = (double)i;
	}

	Geocoordinate *tparr = new Geocoordinate[Nheights];
	LookingDirection ld;
	for(int i=0; i<Nheights; i++){
		ld.set( ld_alpha, heights[i]/m2km );/* 見る場所決め */
		tparr[i] = ld.tangential_point( earth, himawari);/* tangential point の配列 */
	}
	ParamAtmosphere* pAtm;
	std::cout << "calling msis..." << std::endl;
	pAtm = get_msis(dt, tparr, Nheights);/* tangential point でのMSIS大気から求めたパラメタを取得 */

	std::string header = "";
	int optimized_ranges = 0;
	int Ncol = 0;
	double** optimized_vector = readwrite::read_data(PATH, header, optimized_ranges, Ncol);
	if(Ncol != 2){ std::cerr << "error! file format " << std::endl; return 1; }

	for(int ii = 0; ii < optimized_ranges; ++ii){
		int atm_i_top = iTOP - ii*HEIGHT_RANGE;
		int atm_i_bottom = 0;
		double* Nair_arr = new double[Nheights];
		for(int i=atm_i_top+1; i<Nheights; i++){
			Nair_arr[i] = pAtm[i].Nair;
		}
		for(int i=0; i<=atm_i_top; i++){
			Nair_arr[i] = pAtm[atm_i_top+1].Nair * std::pow(10, optimized_vector[1][ii]*(pAtm[i].z - pAtm[atm_i_top+1].z));/* Coef は対数の直線の傾き */
		}
		for(int i=0; i<Nheights; i++){
			pAtm[i].Nair = Nair_arr[i];
			pAtm[i].set_p_from_Nair_T();
		}
		delete[] Nair_arr;
	}

	saveParamAtmosphere(PATH+"_remaked.dat", pAtm, Nheights, atmosphere_precision);
	std::cout << "# Atmosphere\nz\tp\tT\tNair\tNo3\tNo2\tNh2o\tNco2\tNno2" << std::endl;
	for(int i=0; i<Nheights; i++){
		double* tmp_atm = pAtm[i].returnvector();
		for(int j=0; j<pAtm[i].NoPs(); j++){
			std::cout << tmp_atm[j] << "\t";
		}
		std::cout << std::endl;
	}
	delete[] heights;
	return 0;
}

