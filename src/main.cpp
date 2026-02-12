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
#include "golden_section_search.h"
#include "avoid_dupe.h"

//constexpr int i_top    {64};/* 全体のどこかでは誤差計算に含める */
//constexpr int i_bottom {60};
//constexpr int N_exp_decay_atm {5}; /* この範囲の点数では数密度は指数的に変化するものとする(これごとに分割してもとめる) */
constexpr double super_inv_10_scaleheight = 0.03;/* 初期値 */
char input;

/* ==== main ==== */
/* 
 *
 *
 *
 */
/* ==== */


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

	double XTOL = getConfig(configs, "XTOL", 1.0e-6);/* 最適化終了判定 */
	double XTOL_REL = getConfig(configs, "XTOL_REL", 1.0e-6);/* 最適化終了判定 */
	int FITTING_ADDITION = getConfig(configs, "FITTING_ADDITION", 0);/* フィッティングするために上の層の値を余計に計算する。破棄予定 */
	std::string PATH_OTEHON = getConfig(configs, "PATH_OTEHON", "/lhome/sano2/SANO/research/estimate-profile/Result/Result-12-W5/OTEHON_2022_6_1_3_36.dat");

	std::string OPTIMIZER = getConfig(configs, "OPTIMIZER", "NL");
//	DIR_UVSPEC





/* ==== */
/*
	std::string PATH_ATMOSPHERE = "/home/sano/SANO/research/estimate-profile/atmmod-temporary/afglus-modN.dat";
	PATH_ATMOSPHERE = "/home/sano/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/data/atmmod/afglus.dat";
	PATH_ATMOSPHERE = "/home/sano/SANO/research/estimate-profile/atmmod-temporary/msis-atm.dat";
*/
/* ==== 保存先ディレクトリ作成 ==== */
	
	int ifdupe = avoid_dupe(DIR_RESULT);
	if(ifdupe){
		DIR_RESULT = DIR_RESULT +"_"+ std::to_string(ifdupe);
		secid = secid + std::to_string(ifdupe);
	}
	std::cerr << "create_directory " << DIR_RESULT << std::endl;
	std::filesystem::create_directory(DIR_RESULT);

	if(DEBUG){ std::cin >> input; }

//	std::string path_result = "result.txt";

/* ==== */
/* ==== 観測データ読み込み ==== */
	
	std::string path_obs = obs_path(DIR_OBS, dt);/* 観測日時からデータの名前 */
	std::cout << path_obs << std::endl;
	if(DEBUG){ std::cin >> input; }
	int Nobs = 0;
	Observed *obsds = read_obs( &Nobs, path_obs );/* 使うのはobsds[obs_index]だけ */
	std::cout << Nobs << "points" << std::endl;

//	int otehon_lines;
//	int otehon_columns;
//	std::string otehon_header;
//	double** otehon = fit::read_result(PATH_OTEHON, otehon_header, otehon_lines, otehon_columns);
//	std::cout << "Read Otehon." << std::endl;
//	Observed *obsds = new Observed[obs_index + 1];
//	obsds[obs_index].set(62.0, 69.6, otehon_lines, otehon[0], otehon[4]);/* TODO TODO TODO HARD CODING !!! */
//
//	for(int i=0; i<otehon_lines; i++){
//		std::cout << otehon[0][i] <<" "<< obsds[obs_index].Data()[i] << std::endl;
//	}


/* ==== */
/* 諸定数の準備 */
	WrapperArgs args;/* declared in wrapper.h */
	args.TOA_height           = obsds[obs_index].maxHeight();
	
	std::filesystem::remove(DIR_LOG+"libRadtran.log");/* ログ容量溢れ防止 */

	double *heights = obsds[obs_index].Heights();
	int Nheights = obsds[obs_index].Nheights();
	std::cout << "lat" << obsds[obs_index].Latitude() << " "  << "lon" << obsds[obs_index].Longitude() << " " << obsds[obs_index].Nheights() << "heights max:" << args.TOA_height << std::endl;
//	for(int i=0; i<obsds[obs_index].Nheights(); i++){
//		std::cout << heights[i] << " " << obsds[obs_index].Data(heights[i]) << "\n";
//	}
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
	AndoLab::solar_direction(on_ground.latitude(), on_ground.longitude(), dt.DOY(), dt.HourWithDecimal(), &sza_on_ground, &phi0_on_ground);/* on_ground での太陽方向 */
	
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
//	ParamAtmosphere *pAtm = get_msis(dt, tparr, Nheights);/* tangential point でのMSIS大気から求めたパラメタを取得 */
	ParamAtmosphere* pAtm = readParamAtmosphere(PATH_ATMOSPHERE_INIT, Nheights);/* 初期大気は自分で指定する *//* try-catchが望ましい */
	std::cout << "# Atmosphere\nz Nair p T" << std::endl;
	for(int i=0; i<Nheights; i++){
		std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
	}

//	saveParamAtmosphere(PATH_ATMOSPHERE, pAtm, Nheights, atmosphere_precision);

//	saveParamAtmosphere(PATH_ATMOSPHERE, pAtm, Nheights, atmosphere_precision);
	args.Nheights               = Nheights;
	args.atm_Nheights           = Nheights;
	args.pAtm           = pAtm;/* 初期値 */
	args.atm_heights = new double[Nheights];
	for(int i=0; i<Nheights; i++){
		args.atm_heights[i] = args.pAtm[i].z;
	}
	args.upper_radiance_smoothed       = new double[args.Nheights];
	args.radiance_smoothed             = new double[args.Nheights];
	for(int i=0; i<args.Nheights; i++){
		args.upper_radiance_smoothed[i] = 0.0;
	}
	args.offset_bottom_height = getConfig(configs, "offset_bottom_height", 89.9);/* for fit */
	args.offset_top_height    = getConfig(configs, "offset_top_height", 100.1);/* for fit */
/* ==== */

/* ==== 上から求める ==== */
	int N_repeating_optimization = ((i_top - i_bottom) + N_exp_decay_atm-1) / N_exp_decay_atm;/* 切り上げ除算 *//* 最適化を走らせる回数 */
	double* inv_10_scaleheights = new double[N_repeating_optimization];/* 最適化して求めた係数を保存する */
	for(int i_stage=0; i_stage<N_repeating_optimization; i_stage++){
		int i_top_opt = i_top - i_stage*N_exp_decay_atm;
		int i_bottom_opt = i_top_opt - N_exp_decay_atm+1;
		if( i_bottom_opt < i_bottom ){ i_bottom_opt = i_bottom; }
		
		args.DIR_RESULT = DIR_RESULT+"/"+std::to_string(i_stage);/* for save */
		std::cout << "create_directory " << args.DIR_RESULT << std::endl;
		std::filesystem::create_directory(args.DIR_RESULT);
		args.secid                = secid+"_"+std::to_string(i_stage);/* for save */
	
	/* ==== prepare for wrapper ==== */
	//	args.pStdin = pStdin;
		args.dt                     = dt;
		args.obs                    = obsds[obs_index];/* for fitting (and save) */
		args.planet                 = earth;
		args.satellite              = himawari;
		args.heights                = args.obs.Heights();/* for save */
		args.on_ground              = on_ground;/* for save */
		args.sza_on_ground          = sza_on_ground;/* for save */
		args.phi0_on_ground         = phi0_on_ground;/* for save */
		args.tparr                  = tparr;/* tangential points */
		args.DIR_UVSPEC             = DIR_UVSPEC;
		args.PATH_STDIN             = PATH_STDIN;
		args.PATH_STDOUT            = PATH_STDOUT;
		args.PATH_ATMOSPHERE        = PATH_ATMOSPHERE;
		args.PATH_CONFIG            = PATH_CONFIG;/* for save */
		args.FLAG_UNDISPLAY_LOG     = FLAG_UNDISPLAY_LOG;
		args.DIR_LOG                = DIR_LOG;
		args.i_bottom               = i_bottom_opt;/* 誤差計算に含める最低 */
		args.i_top                  = i_top_opt;   /* 誤差計算に含める最高 */
		args.fit_i_bottom           = args.i_bottom;/* fitに含める最高 */
//		args.fit_i_top              = i_top;/* TODO 上はフィッティングに全部含める *///args.i_top + FITTING_ADDITION;   /* fitに含める最低 */
		args.fit_i_top              = args.i_top;	

		args.atmosphere_precision = atmosphere_precision;
		args.obs_index            = obs_index;/* for save */
		args.N_running_mean       = 3;/*移動平均*/
		for(int i=0; i<args.Nheights; i++){
			args.radiance_smoothed[i] = 0.0;
		}
	/* ==== */
	/* MSISで求めた大気をNLoptの初期値に代入する。最小化する評価関数はwrapperとして実装するが、
	*/
		int running_mean_extra = args.N_running_mean / 2;/* ( N - 1 ) / 2 */
	
		args.atm_i_bottom = 0; /* TODO 今は 地表まで	*/
		args.atm_i_top    = args.i_top   ;// + running_mean_extra;
	
	/* ==== Optimize ==== */
	/* -- 各点 -- */
	//	int number_of_optimization_parameters = args.atm_i_top - args.atm_i_bottom + running_mean_extra;//1;//args.atm_i_top - args.atm_i_bottom + 1;
	//	std::vector<double> x(number_of_optimization_parameters, 0.0);/* 初期値 */
	//	for(int i=args.atm_i_bottom; i<=args.atm_i_top; i++){/* 初期化 */
	//		x[i - args.atm_i_bottom] = std::log10(pAtm[i].Nair);/* Coef は対数 */
	//	}
	/* -- 直線1つのみ -- */
		int number_of_optimization_parameters = 1;
		std::vector<double>x(number_of_optimization_parameters, -0.1);
		x[0] = -super_inv_10_scaleheight;/* 初期値 initial value */
	/* -- 各点(下限を直上の層とする) -- */
	//	int number_of_optimization_parameters = args.atm_i_top - args.atm_i_bottom + running_mean_extra;//1;//args.atm_i_top - args.atm_i_bottom + 1;
	//	std::vector<double> x(number_of_optimization_parameters, 0.0);/* 初期値 */
	//	for(int i=args.atm_i_bottom; i<=args.atm_i_top; i++){/* 初期化 */
	//		x[i - args.atm_i_bottom] = std::log10(pAtm[i].Nair) - std::log10(pAtm[i-1].Nair);/* Coef は対数 */
	//		if(x[i - args.atm_i_bottom] > 0.0){ x[i - args.atm_i_bottom] = 0.0; }
	//	}



/* ==== optimization ==== */
		double minf;
		
		if(OPTIMIZER == "NL"){
			nlopt::opt opt( nlopt::LN_NELDERMEAD, number_of_optimization_parameters );
			opt.set_min_objective( wrapper, (void*)(&args) ); 
			opt.set_xtol_rel(XTOL_REL);/* TODO */
			//opt.set_xtol_abs(XTOL_REL);/* TODO */
		//	opt.set_lower_bounds(10);/* TODO */
			opt.set_upper_bounds(0.0);/* 必ず上が減少 */
			opt.set_lower_bounds(-0.21);/* TODO 今は MSISの4倍程度 */
			try {
				args.number_of_iteration = 0;
				/* ---- NLopt ---- */
				nlopt::result result = opt.optimize(x, minf);
				std::string nlopt_result = get_nlopt_result_description(result);
				std::string nlopt_result_code = get_nlopt_result_string(result);
				std::cerr << nlopt_result << std::endl;
				/* ---- */
				std::string path_save_vector = args.DIR_RESULT+"/optimized_vector.dat";
				std::ofstream save_vector (path_save_vector);
				if(!save_vector){
					std::cerr << "main: optimized vector_error cannot be saved!! path: " << path_save_vector << std::endl;
				}
				inv_10_scaleheights[i_stage] = x[0];
				for(int i=0; i<N_repeating_optimization; i++){
		//			for(number_of_optimization_parameters)
					save_vector << i <<" "<< inv_10_scaleheights[i] << nlopt_result_code << std::endl;
				}
				save_vector.close();
			} catch (std::exception &e){
				std::cout << "NLopt failed : " << e.what() << std::endl;
			}
		}
		else if(OPTIMIZER == "golden"){
			double x_opt;
			double lower_bound = 2.0 * -0.0523572;
			/* ---- golden section search ---- */
			minf = golden_section_search( x_opt, lower_bound, 0.0, XTOL, wrapper, (void*)(&args) );
			/* ---- */
			std::string path_save_vector = args.DIR_RESULT+"/optimized_vector.dat";
			std::ofstream save_vector (path_save_vector);
			if(!save_vector){
				std::cerr << "main: optimized vector_error cannot be saved!! path: " << path_save_vector << std::endl;
			}
			inv_10_scaleheights[i_stage] = x_opt;
			for(int i=0; i<N_repeating_optimization; i++){
				save_vector << i <<" "<< inv_10_scaleheights[i] << std::endl;
			}
		}
/* ==== */
		for(int i=0; i<args.Nheights; i++){
//TODO				args.upper_radiance[i] = args.radiance[i];
		}
		
	}	

//	delete[] radiance;
	return 0;
}

