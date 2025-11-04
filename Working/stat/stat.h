/*
 * stat.h
 * 
 */

#ifndef __FIT_RESULT_H__
#define __FIT_RESULT_H__

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

#include<cmath>
#include <functional>
#include<nlopt.hpp>
// #include<algorithm>


	/* ---- stat_readwrite.cpp ---- */
	double** read_result(std::string path, std::string &header, int &Nlines, int &Ncolumns);/* 結果を単純に2次元配列に格納する */
	void save_merged_result(std::string path, std::string header, int Nlines, int Ncolumns, double* index, double** data);
	/* ---- */
	/* ---- mean.cpp ---- */
	double mean(std::string path);/* path->data[1]の平均 */
	double mean(double*data, int Ndata);
	/* ---- */

#endif
