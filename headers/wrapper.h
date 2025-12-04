/*
 *
 * NLoptに渡す関数を作るwrapper function
 *
 */

#include <nlopt.hpp>

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
#include "fit.h"

/* MSIS_TO_GM_E */

double* msis_to_gm_e(
	ParamAtmosphere* patm,
	PlanetParam earth,
	obsDateTime dt,
	Geocoordinate* coord,
	int Nheights
);




class WrapperArgs{/* wrapperに渡すものをすべてまとめる（NLoptが要求する形式にするため） */
private:
public:
	ParamStdin       pStdin;
	ParamAtmosphere* pAtm;
	obsDateTime      dt;
	Observed         obs;/* for fitting (and save) */
	PlanetParam      planet;
	SatelliteParam   satellite;
	int              Nheights;
	int              atm_Nheights;
	double*          heights;/* for fitting and save */
	Geocoordinate    on_ground;/* for save */
	double           sza_on_ground;/* for save */
	double           phi0_on_ground;/* for save */
	Geocoordinate*   tparr;/* tangential points */
	std::string      DIR_UVSPEC;
	std::string      PATH_STDIN;
	std::string      PATH_STDOUT;
	std::string      PATH_ATMOSPHERE;
	std::string      DIR_RESULT;/* for save */
	std::string      PATH_CONFIG;/* for save */
	int              FLAG_UNDISPLAY_LOG;
	std::string      DIR_LOG;
	int              i_bottom;/* for fit, error */
	int              i_top;/* for fit, error */
//	double min_height;/* for fit, けずれる */
//	double max_height;/* for fit, けずれる */
	int              atm_i_bottom;
	int              atm_i_top;
	double           TOA_height;
	double           offset_bottom_height;/* for fit */
	int              atmosphere_precision;
	std::string      secid;/* for save */
	int              obs_index;/* for save */
	int              N_running_mean;/* for fit */
	int              number_of_iteration;/* NLopt */
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
