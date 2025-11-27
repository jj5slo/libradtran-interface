/*
 * wrapper.cpp
 *
 *
 */

/*
 *	すべきこと
 *			
 *		fitを組み込む
 * 		
 */


#include"wrapper.h"



double wrapper(const std::vector<double> &Coef, std::vector<double> &grad, void* raw_Args){
	constexpr int ITERATION_LIMIT = 3000;
	WrapperArgs* args = static_cast<WrapperArgs*>(raw_Args);
//	std::cout <<std::endl<< args->Nheights;
//	std::cout <<std::endl<< args->atm_Nheights;
//	std::cout <<std::endl<< args->DIR_UVSPEC;
//	std::cout <<std::endl<< args->PATH_STDIN;
//	std::cout <<std::endl<< args->PATH_STDOUT;
//	std::cout <<std::endl<< args->PATH_ATMOSPHERE;
//	std::cout <<std::endl<< args->DIR_RESULT;/* for save */
//	std::cout <<std::endl<< args->PATH_CONFIG;/* for save */
//	std::cout <<std::endl<< args->FLAG_UNDISPLAY_LOG;
//	std::cout <<std::endl<< args->DIR_LOG;
//	std::cout <<std::endl<< args->i_bottom;/* for fit, error */
//	std::cout <<std::endl<< args->i_top;/* for fit, error */
//	std::cout <<std::endl<< args->atm_i_bottom;
//	std::cout <<std::endl<< args->atm_i_top;
//	std::cout <<std::endl<< args->TOA_height;
//	std::cout <<std::endl<< args->offset_bottom_height;/* for fit */
//	std::cout <<std::endl<< args->atmosphere_precision;
//	std::cout <<std::endl<< args->secid;/* for save */
//	std::cout <<std::endl<< args->obs_index;/* for save */
//	std::cout <<std::endl<< args->N_running_mean;/* for fit */
//	std::cout <<std::endl<< args->number_of_iteration;/* NLopt */
//	std::cout << std::endl;
	const int running_mean_extra = args->N_running_mean / 2;/* ( N - 1 ) / 2 */
	const int i_bottom_rad = args->i_bottom - running_mean_extra;
	const int i_top_rad = args->i_top + running_mean_extra;
	
	std::cerr << "Iteration " << args->number_of_iteration << "th time" << std::endl;

	/* ==== setting atmosphere ==== */
	std::cout << "setting atmosphere..." << std::endl;
	double* Nair_arr = new double[args->atm_Nheights];
	for(int i=0; i<args->atm_i_bottom; i++){
		Nair_arr[i] = args->pAtm[i].Nair;
	}
	for(int i=args->atm_i_bottom; i<=args->atm_i_top; i++){/* TODO 一時的に、top_rad 以上は真値としている */
		Nair_arr[i] = Coef[args->atm_i_top - i];/* 上から */
	}
	for(int i=args->atm_i_top+1; i<args->atm_Nheights; i++){
		Nair_arr[i] = args->pAtm[i].Nair;
	}
	args->pAtm = Nair_to_atmosphere(
		args->atm_Nheights,
		args->dt,
		args->tparr,
		args->planet,
		Nair_arr,
		0.0
		);	
		saveParamAtmosphere(args->PATH_ATMOSPHERE, args->pAtm, args->atm_Nheights, args->atmosphere_precision);
	/* ==== */
	double *radiance = new double [args->Nheights];/* シミュレーション結果 */
	for(int j=0; j<args->Nheights; j++){
		radiance[j] = 0.0;/* initialize */
	}
	for(int i=i_bottom_rad; i<=i_top_rad; i++){
	/* ==== setting sensor ==== */
		std::cout << "setting sensor..." << std::endl;
		Geocoordinate tp = args->tparr[i];
		std::cout << "Tangential point:\n";
		std::cout << "\tlat:" << tp.latitude() << "\n\tlon:" << tp.longitude() << "\nt\taltitude:" << tp.altitude() << "\n\talpha:" << tp.alpha()*Rad2deg << std::endl;


		AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
		crosspts = Across_point_atmosphere(args->planet, args->satellite, tp.r(), args->TOA_height); 
		Geocoordinate crosspt(args->planet, args->satellite, crosspts[0]);/* TOAと視線の交点を求める */

		std::cout << "crosspoint_of_atmosphere:\n\tlat:" << crosspt.latitude() << "\n\tlon:" << crosspt.longitude() << "\n\taltitude:" << crosspt.altitude() << std::endl;
		
		AndoLab::solar_direction(crosspt.latitude(), crosspt.longitude(), args->dt.DOY(), args->dt.Hour(), &args->pStdin.sza, &args->pStdin.phi0);/* tangential point での太陽方向を求める */
		std::cout << "sza:" << args->pStdin.sza << " phi0:" << args->pStdin.phi0 << std::endl;
		sensor_direction(args->satellite, crosspt.r(), &args->pStdin.umu, &args->pStdin.phi);/* crosspt からみた衛星方向を元に、視線方向の局所鉛直からの角（オフナディア角）を求める */
		double sensor_theta;
		sensor_theta = acos(args->pStdin.umu) * Rad2deg;
		std::cout << "sonsor_direction:\n\tsensor_theta:" << sensor_theta <<"\n\tphi:" << args->pStdin.phi << std::endl;
		std::cout << "cos(sensor_theta) = umu = " << args->pStdin.umu << std::endl;
	
		save_stdin(args->PATH_STDIN, args->pStdin);/* 座標情報を入力ファイルにセーブ */
	/* ==== */
	/* ==== acquiring radiance from libRadtran ==== */	
	std::cout << "acquiring radiance from libRadtran..." << std::endl;	
		/* delete_mystic_rad(); */
		execute_uvspec(args->DIR_UVSPEC, args->PATH_STDIN, args->PATH_STDOUT, args->FLAG_UNDISPLAY_LOG, args->DIR_LOG);
		if(args->pStdin.solver == "mystic"){
			radiance[i] = read_mystic_rad(args->DIR_UVSPEC, 105);/* TODO この層番号の決め方がいまいちわからない 0から100km, 1kmごとであればTOAで105 */
		}else{
			radiance[i] = read_stdout(args->PATH_STDOUT, 0);
		}	
	/* ==== */
		std::cout << "Tangential height: " << tp.altitude() << " [m]\nRadiance: " << radiance[i] << "\n----" << std::endl;
	}

/* ==== saving results ==== */
	std::cout << "saving results..." << std::endl;
	std::string path_result = save_path(args->DIR_RESULT, args->secid, args->dt, args->obs_index + 1);
	save_result(path_result, args->secid, args->on_ground, args->Nheights, args->heights, radiance);/* TODO 最適化を回し始めたらいらない */
	save_params(args->DIR_RESULT, args->secid, args->PATH_ATMOSPHERE, "_atm"+std::to_string(args->number_of_iteration)+".dat");/* atmosphereも保存しておく */
	save_params(args->DIR_RESULT, args->secid, args->PATH_STDIN, "_stdin.txt");
	save_params(args->DIR_RESULT, args->secid, args->PATH_CONFIG, "_config.conf");
/* ==== */
/* ==== fitting results ==== */
	std::cout << "fitting results..." << std::endl;
	double offset = fit::mean(args->Nheights, args->heights, args->obs.Data(), args->offset_bottom_height, args->TOA_height);
	double* a_offset = fit::obtain_fitting_coefficient(args->obs.Data(), radiance, args->i_bottom, args->i_top, offset);
	double* fitted = fit::apply_fitting(args-> Nheights, radiance, a_offset);
	double* smoothed = fit::running_mean_log(args->Nheights, args->N_running_mean, fitted);/* 3 は移動平均をとる数 */
	double** fitted_results = new double* [5];
	fitted_results[0] = args->heights;
	fitted_results[1] = args->obs.Data();
	fitted_results[2] = radiance;
	fitted_results[3] = fitted;
	fitted_results[4] = smoothed;
	double ld_alpha = args->on_ground.alpha();
	std::string header = 
		"# secid: " + args->secid + "\n"
		+"# number_of_iteration: " + std::to_string(args->number_of_iteration) + "\n"
		+ "# longitude: " + std::to_string(args->on_ground.longitude()) + "\n"
		+ "# latitude: " + std::to_string(args->on_ground.latitude()) + "\n"
		+ "# ld_alpha: " + std::to_string(ld_alpha) + "[rad] = " + std::to_string(ld_alpha*Rad2Deg) + "deg" + "\n"
		+ "# i_bottom: " + std::to_string(args->i_bottom) + ", i_top: " + std::to_string(args->i_top) + "\n"
		+ "# atm_i_bottom: " + std::to_string(args->atm_i_bottom) + ", atm_i_top: " + std::to_string(args->atm_i_top) + "\n"
		+ "# a: " + std::to_string(a_offset[0]) + ", offset: " + std::to_string(a_offset[1]) + "\n"
		+ "# N_running_mean: " + std::to_string(args->N_running_mean) + "\n";
	fit::save_data(path_result+"_fitted.dat", header, args->Nheights,  5, fitted_results);
	delete[] fitted_results;

	double log_square_error = fit::root_mean_square_log_error( args->i_bottom, args->i_top, args->obs.Data(), smoothed );
	
//	double grad_err = 0;/* TODO(むり) */
	args->number_of_iteration++;
	if( args->number_of_iteration > ITERATION_LIMIT ){
		std::cerr << "wrapper: iteration by nlopt stopped." << std::endl;
		throw nlopt::forced_stop();
	}
	if( !grad.empty() ){/* 微分を使わないアルゴリズムもある */
//		grad[0] = grad_err;
		std::cerr << "wrapper: Can't calculate gradient!!" << std::endl;
		throw nlopt::forced_stop();
	}
	return log_square_error;	
}


