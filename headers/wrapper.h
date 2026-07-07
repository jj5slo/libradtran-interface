/*
 *
 * NLoptに渡す関数を作るwrapper function
 *
 */

#ifndef __WRAPPER_H__
#define __WRAPPER_H__


#include <nlopt.hpp>

#include <iostream>
#include <cstdlib>
#include <algorithm>/* 最大最小用 */
#include <chrono>
#include <filesystem>
#include <bayesopt/bayesopt.hpp>

#include "solar_direction.h"

#include "coordinate.h"
#include "interface.h"
#include "execute.h"
#include "save.h"
#include "get_msis.h"
#include "read_config.h"
#include "fit.h"
#include "SpectralResponse.h"

/* -- optimize_util.cpp -- */
std::string get_nlopt_result_description(nlopt::result res);
std::string get_nlopt_result_string(nlopt::result res);

/* MSIS_TO_GM_E */

double* msis_to_gm_e(
	ParamAtmosphere* patm,
	PlanetParam earth,
	obsDateTime dt,
	Geocoordinate* coord,
	int Nheights
);




// class WrapperArgValues{
// private:
// public:
// 
// }


class WrapperArgs/* : public WrapperArgValues */{/* wrapperに渡すものをすべてまとめる（NLoptが要求する形式にするため） */
private:
public:
	WrapperArgs();
	~WrapperArgs();
//	WrapperArgs(const WrapperArgs &obj);
//	WrapperArgs& operator=(const WrapperArgs& obj);
/* 値コピーでいいものをstructにまとめ、全部関数で返すように */

	ParamStdin       pStdin;
	ParamAtmosphere* pAtm;
	obsDateTime      dt;
	Observed         obs;              /* for fitting (and save) */
	PlanetParam      planet;
	SatelliteParam   satellite;
	int              Nheights;
	int              atm_Nheights;     
	double*          heights;          /* for fitting and save */
	double*          atm_heights;      /* atmosphere */
	Geocoordinate    on_ground;        /* for save */
	double           sza_on_ground;    /* for save */
	double           phi0_on_ground;   /* for save */
	Geocoordinate*   tparr = nullptr;  /* tangential points */
	std::string      DIR_UVSPEC  = "";
	std::string      PATH_STDIN  = "";
	std::string      PATH_STDOUT = "";
	std::string      PATH_ATMOSPHERE;
	SpectralResponseWeights SRWeights;
	std::string      DIR_RESULT;       /* for save */
	std::string      PATH_CONFIG;      /* for save */
	int              FLAG_UNDISPLAY_LOG;
	std::string      DIR_LOG;
	int              i_bottom;         /* for error */
	int              i_top;            /* for error */
	int              fit_i_bottom;     /* for fit */
	int              fit_i_top;        /* for fit */
//	double min_height;/* for fit, けずれる */
//	double max_height;/* for fit, けずれる */
	int              atm_i_bottom;
	int              atm_i_top;
	double           TOA_height;
//	double           offset_bottom_height;/* for fit */
//	double           offset_top_height;/* for fit */
	int              FLAG_adapt_mc_photons = 0;
	int              atmosphere_precision;
	std::string      secid;               /* for save */
	int              obs_index;           /* for save */
	int              N_running_mean;      /* for fit */
	int              number_of_iteration = 0; /* NLopt */
	double*          radiance_smoothed;
	double*          upper_radiance_smoothed;
	double           xtol_rel; /* for BayesOpt */
};

//	args->pStdin;
//	args->pAtm;
//	args->dt;
//	args->obs;/* for fitting (and save) */
//	args->planet;
//	args->satellite;
//	args->Nheights;
//	args->atm_Nheights;
//	args->heights;/* for save */
//	args->on_ground;/* for save */
//	args->tparr;/* tangential points */
//	args->DIR_UVSPEC;
//	args->PATH_STDIN;
//	args->PATH_STDOUT;
//	args->PATH_ATMOSPHERE;
//	args->SRWeights;
//	args->DIR_RESULT;/* for save */
//	args-> PATH_CONFIG;/* for save */
//	args->FLAG_UNDISPLAY_LOG;
//	args->DIR_LOG;
//	args->i_bottom;
//	args->i_top;
//	args->atm_i_bottom;
//	args->atm_i_top;
//	args->TOA_height;
//	args->offset_bottom_height;/* for fit */
//	args->atmosphere_precision;
//	args->secid;/* for save */
//	args->obs_index;/* for save */
//	args->number_of_iteration;/* NLopt */

double wrapper(const std::vector<double> &Coef, std::vector<double> &grad, void* raw_Args_to_be_converted_to_WrapperArgs_pointer);/* for NLopt */

inline double bo_wrapper(unsigned int n, const double *Coef, double *grad, void* raw_Args_to_be_converted_to_WrapperArgs_pointer) {
	std::vector<double> vec_Coef(Coef, Coef + n);
	std::vector<double> vec_grad;
	if (grad != nullptr) {
		vec_grad.resize(n);
	}
	
	// 元の重いシミュレーション（NLopt用のwrapper）を呼び出し
	double result = wrapper(vec_Coef, vec_grad, raw_Args_to_be_converted_to_WrapperArgs_pointer);
	
	if (grad != nullptr) {
		for (unsigned int i = 0; i < n; ++i) {
			grad[i] = vec_grad[i];
		}
	}
	
	return result;
}
class BO_WrapperModel : public bayesopt::ContinuousModel {
private:
	void* wrapper_args;

public:
	/* Constructor */
	BO_WrapperModel(bopt_params params, void* args) : ContinuousModel(1, params), wrapper_args(args) {}

	double evaluateSample(const vectord &Xi) override {
		std::vector<double> Coef(Xi.size());
		for(size_t i = 0; i < Xi.size(); ++i){
			Coef[i] = Xi[i];
		}
		std::vector<double> grad;

		double result_y = wrapper(Coef, grad, wrapper_args);
		
		return result_y;
	}
};



/* wrapper では、輝度計算と観測光強度にフィッティング・誤差の算出以外に、各高度に対するセンサ向きの設定を行う必要がある。更新する入力ファイルは大気プロファイルと、標準入力。 */
double core(void* raw_Args_to_be_converted_to_WrapperArgs_pointer);/* 単純に今の設定ファイルで一回実行するだけ */

//double acquire_radiance(
//	const std::string DIR_UVSPEC,
//	const std::string PATH_STDIN,
//	const std::string PATH_STDOUT,
//	const int FLAG_UNDISPLAY_LOG,
//	const std::string solver
//);

ParamAtmosphere* Nair_to_atmosphere(
	int atm_Nheights,
	obsDateTime dt,
	Geocoordinate*& coord,
	PlanetParam earth,
	double* x,
	const double PRESSURE_AT_TOA//,
//	double* gm_e_arr 
);

#endif
