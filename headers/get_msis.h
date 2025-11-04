/*
 * get_msis.h
 *
 */

#include<iostream>

#include<nrlmsise-00.h>

/* #include "../layered_atmosphere.h" */
#include "interface.h"
#include "obsDateTime.h"
#include "coordinate.h"
/* ParamAtmosphereはinterface.h */

ParamAtmosphere* get_msis(obsDateTime dt, Geocoordinate* coord, int N);/* 日時、各点の座標、coordがいくつあるか */

double get_msis_total_mass_density(/* 全質量密度を返す */
	obsDateTime dt, Geocoordinate coord);
