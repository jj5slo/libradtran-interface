/*
 * squareerr.cpp
 *
 *
 */
#include "fit.h"

double fit::square_log_error(int Ndata, double* obs, double* sim){	
	double err = 0.0;
	double logdiff = 0.0;
	for(int j=0; j < Ndata; j++){/* 対数の二乗誤差をとる */
		logdiff = std::log10(obs[j]) - std::log10(sim[j]);
		err += logdiff*logdiff;
//		std::cerr << err << std::endl;
	}
	return err;
}
double fit::square_log_error(int min_index, int max_index, double* obs, double* sim){
	double err = 0.0;
	double logdiff = 0.0;
	for(int j=min_index; j <= max_index; j++){/* 対数の二乗誤差をとる */
		logdiff = std::log10(obs[j]) - std::log10(sim[j]);
		err += logdiff*logdiff;
	}/* 対数の誤差の二乗和を、指定した高度の範囲内だけでとる */
//		std::cerr << err << std::endl;
	return err;
}
double fit::square_log_error(int Ndata, double min_height, double max_height, double* heights, double* obs, double* sim){
	double err = 0.0;
	double logdiff = 0.0;
	for(int j=0; j < Ndata; j++){/* 対数の二乗誤差をとる */
		if( min_height <= heights[j] && heights[j] <= max_height ){
			logdiff = std::log10(obs[j]) - std::log10(sim[j]);
			err += logdiff*logdiff;
		}/* 対数の誤差の二乗和を、指定した高度の範囲内だけでとる */
//		std::cerr << err << std::endl;
	}
	return err;
}
double fit::root_mean_square_log_err(int Ndata, double* heights, double* obs, double* sim){
	double sqle = fit::square_log_error(Ndata, obs, sim);
	return std::sqrt( sqle / Ndata );
}
double fit::root_mean_square_log_err(int min_index, int max_index, double* obs, double* sim){
	int Ntarget = max_index - min_index + 1;
	double sqle = fit::square_log_error(min_index, max_index, obs, sim);
	return std::sqrt( sqle / Ntarget );
}
double fit::root_mean_square_log_err(int Ndata, double min_height, double max_height, double* heights, double* obs, double* sim){
	int Ntarget = 0;
	for(int i=0; i<Ndata; i++){
		if( min_height <= heights[i] && heights[i] <= max_height ){
			Ntarget++;
		}
	}
	double sqle = fit::square_log_error(Ndata, min_height, max_height, heights, obs, sim);
	return std::sqrt( sqle / Ntarget );
}

