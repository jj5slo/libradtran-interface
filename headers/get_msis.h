/*
 * get_msis.h
 *
 */
#ifndef __GET_MSIS_H__
#define __GET_MSIS_H__

#include<iostream>

#include<nrlmsise-00.h>

/* #include "../layered_atmosphere.h" */
#include "interface.h"
#include "obsDateTime.h"
#include "coordinate.h"
/* ParamAtmosphereはinterface.h */

constexpr double NA {6.02214076e23};

ParamAtmosphere* get_msis(obsDateTime dt, Geocoordinate* coord, int N);/* 日時、各点の座標、coordがいくつあるか */

double get_msis_total_mass_density(/* 全質量密度を返す */
	obsDateTime dt, Geocoordinate coord);
double get_msis_total_mass_density(/* 全質量密度を返す */
	obsDateTime dt, Geocoordinate coord1, Geocoordinate coord2);
double get_msis_average_molecular_weight(/* 平均分子量を返す */
	obsDateTime dt, Geocoordinate coord);
double get_msis_average_molecular_weight(/* 平均分子量を返す */
	obsDateTime dt, Geocoordinate coord1, Geocoordinate coord2);


#endif
