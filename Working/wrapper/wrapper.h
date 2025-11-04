/*
 *
 * NLoptに渡す関数を作るwrapper function
 *
 */

#include<nlopt.hpp>

#include<iostream>
#include<cstdlib>
#include<algorithm>/* 最大最小用 */
#include<chrono>
#include<filesystem>

#include"solar_direction.h"

#include"coordinate.h"
#include"interface.h"
#include"execute.h"
#include"save.h"
#include"get_msis.h"
#include"read_config.h"





class WrapperArgs{/* wrapperに渡すものをすべてまとめる（NLoptが要求する形式にするため） */
private:
public:
	ParamStdin pStdin;
	ParamAtmosphere pAtm;
	const int Nheights;
	const std::string DIR_UVSPEC;
	const std::string PATH_STDIN;
	const std::string PATH_STDOUT;
	const int FLAG_UNDISPLAY_LOG;
};



double acquire_radiance(
	const std::string DIR_UVSPEC,
	const std::string PATH_STDIN,
	const std::string PATH_STDOUT,
	const int FLAG_UNDISPLAY_LOG,
	const std::string solver
);

ParamAtmosphere* Nair_to_atmosphere(
	int atm_Nheights,
	obsDateTime dt,
	Geocoordinate*& coord,
	PlanetParam earth,
	std::vector<double> x,
	const double PRESSURE_AT_TOA
);
