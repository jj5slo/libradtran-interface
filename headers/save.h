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
#include<filesystem>

#include"Observed.h"
#include"obsDateTime.h"
#include"coordinate.h"
#include"interface.h"

constexpr double Rad2Deg = 180/M_PI;

std::string save_path(
	const std::string data_dir,
	const std::string secid,
	const obsDateTime dt,
	double ld_alpha,
	int line_index);/* 結果の保存名 */
std::string save_path(
	const std::string data_dir,
	const std::string secid,
	const obsDateTime dt,
	const int line_index);/* 結果の保存名 */

int save_result(
	std::string path,
	std::string secid,
	Geocoordinate on_ground,
	int Nheights,
	double* heights,
	double* radiance
);/* 結果保存 */
int save_result(
	std::string path,
	std::string secid, 
	Geocoordinate on_ground, 
	int Nheights, 
	double* heights, 
	Observed obsd, 
	double* radiance
);/* 結果保存 */

int save_params(std::string dir, std::string secid, std::string path_stdin, std::string suffix);/* パラメータ保存(inファイルをコピーするだけ) */

#endif
