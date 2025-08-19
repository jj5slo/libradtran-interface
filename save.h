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

#include"Observed.h"
#include"obsDateTime.h"

std::string save_path(std::string path, obsDateTime dt);

int save_result(std::string path, int Nheights, double* heights, Observed obsd, double* radiance);


#endif
