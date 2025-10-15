/*
 * fit.h
 * 複数時点のシミュレーション結果から、フィッティング係数を求める
 */

#ifndef __FIT_H__
#define __FIT_H__

#include<iostream>
#include<string>
#include<fstream>
#include<sstream>

#include<cmath>

// #include<algorithm>

namespace fit
{

	/* ---- fit_result.cpp ---- */	
	constexpr int LS = 0; /* そのまま最小二乗法 */
	constexpr int LOG_LS = 1; /* 対数をとった上で最小二乗法 */
	
	double* fitting_result(int Ndata, double* obs, double* sim, int TYPE);/* 観測(フィッティング先)データ, シミュレーション(被フィッティング)データ, フィッティングの種類 */
	
	double* fitting_result(std::string path, int TYPE, int* Ndata); /*saveで作ったファイルから読み込む. ファイルに保存もする */
	
	/* ---- */
	/* ---- fit_readwrite.cpp ---- */
	double** read_result(std::string path, std::string *header, int *Nlines, int *Ncolumns);/* 結果を単純に2次元配列に格納する */
	void save_fitting_result(std::string path, std::string header, double** data, double* fitted);
	/* ---- */
	
	/* ---- leastsquare.cpp ---- */
	double* leastsquare(double** data, int Nlines);/* data = [i, x, y]. y will be fitted to x. y' = ay + b の 係数 a, b を返す. */
	/* ---- */

}
#endif
