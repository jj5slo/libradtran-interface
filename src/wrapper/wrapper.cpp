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
	constexpr int ITERATION_LIMIT = 3;
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
//	const int running_mean_extra = args->N_running_mean / 2;/* ( N - 1 ) / 2 */
//	const int i_bottom_rad = args->fit_i_bottom - running_mean_extra;
//	const int i_top_rad = args->fit_i_top + running_mean_extra;
	
	std::cerr << args->secid <<" Iteration " << args->number_of_iteration << "th time" << std::endl;

	/* ==== setting atmosphere ==== */
	std::cout << "setting atmosphere..." << std::endl;
	double* Nair_arr = new double[args->atm_Nheights];
/* -- 各点 -- */
//	for(int i=0; i<args->atm_i_bottom; i++){
//		Nair_arr[i] = args->pAtm[i].Nair;
//	}
//	for(int i=args->atm_i_bottom; i<=args->atm_i_top; i++){
//		Nair_arr[i] = std::pow(10, Coef[i - args->atm_i_bottom]);/* Coef は対数 */
//	}
//	for(int i=args->atm_i_top+1; i<args->atm_Nheights; i++){
//		Nair_arr[i] = args->pAtm[i].Nair;
//	}
/* -- 直線1つのみ -- */
	for(int i=args->atm_i_top+1; i<args->atm_Nheights; i++){
		Nair_arr[i] = args->pAtm[i].Nair;
	}
	for(int i=args->atm_i_bottom; i<=args->atm_i_top; i++){
		Nair_arr[i] = args->pAtm[args->atm_i_top+1].Nair * std::pow(10, Coef[0]*(args->pAtm[i].z - args->pAtm[args->atm_i_top+1].z));/* Coef は対数の直線の傾き */
	}
	for(int i=0; i<args->atm_i_bottom; i++){
		Nair_arr[i] = args->pAtm[i].Nair;
	}
/* -- 各点(下限を直上の層とする) -- */
//	for(int i=0; i<args->atm_i_bottom; i++){
//		Nair_arr[i] = args->pAtm[i].Nair;
//	}
//	for(int i=args->atm_i_bottom; i<=args->atm_i_top; i++){/* TODO 一時的に、top_rad 以上は真値としている */
//		Nair_arr[i] = std::pow(10, Coef[i - args->atm_i_bottom]) * Nair_arr[i-1];/* Coef は対数 */
//	}
//	for(int i=args->atm_i_top+1; i<args->atm_Nheights; i++){
//		Nair_arr[i] = args->pAtm[i].Nair;
//	}


	for(int i=0; i<args->atm_Nheights; i++){
		args->pAtm[i].Nair = Nair_arr[i];
		args->pAtm[i].set_p_from_Nair_T();
	}
//	args->pAtm = Nair_to_atmosphere(
//		args->atm_Nheights,
//		args->dt,
//		args->tparr,
//		args->planet,
//		Nair_arr,
//		0.0
//		);	
		saveParamAtmosphere(args->PATH_ATMOSPHERE, args->pAtm, args->atm_Nheights, args->atmosphere_precision);
	/* ==== */

	double log_square_error = core(raw_Args);
	
	std::cerr << "wrapper: log_square_error = " << log_square_error << std::endl;
	std::string path_save_lse = args->DIR_RESULT+"/log_square_error.dat";
	std::ofstream save_lse (path_save_lse, std::ios::app);
	if(!save_lse){
		std::cerr << "wrapper: log_square_error cannot be saved!! path: " << path_save_lse << std::endl;
	}
	save_lse << args->number_of_iteration <<" "<< Coef[0] <<" "<< log_square_error << std::endl;
	save_lse.close();

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


