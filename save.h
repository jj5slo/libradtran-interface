/*
 * save.h
 */
#ifndef __SAVE_H__
#define __SAVE_H__

#include<iostream>
#include<fstream>
#include<string>
#include<iomanip>
#include<sstream>
#include<algorithm>/* minmax用 */
#include<cmath>

#include"Observed.h"
#include"obsDateTime.h"
#include"coordinate.h"

constexpr double Rad2Deg = 180/M_PI;

std::string save_path(std::string path, obsDateTime dt, double ld_alpha);

int save_result(std::string path, Geocoordinate on_ground, int Nheights, double* heights, Observed obsd, double* radiance);


#endif
