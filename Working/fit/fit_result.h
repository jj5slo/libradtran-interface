/*
 * fit_result.h
 * 複数時点のシミュレーション結果から、フィッティング係数を求める
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

namespace fit
{

	/* ---- fit_result.cpp ---- */	
	constexpr int LS = 0; /* そのまま最小二乗法 */
	constexpr int LS_OFFSET = 1;/* オフセットは与えたものを使って a だけ最小二乗法 */
	constexpr int LOG = 2; /* 対数の差の二乗が最小になるように ay+b の aとbを決める。実態はNLOptで最適化。 */
	
	double* fitting_result(int Ndata, double* obs, double* sim, double min_height, double max_height, double offset, int TYPE);/* 観測(フィッティング先)データ, シミュレーション(被フィッティング)データ, フィッティングに使用する最小高度, 最大高度, オフセット, フィッティングの種類 */
	
	double* fitting_result(std::string path, double min_height, double max_height, double offset, int TYPE, int* Ndata); /* saveで作ったファイルから読み込む. ファイルに保存もする */
	
	/* ---- */
	/* ---- fit_readwrite.cpp ---- */
	double** read_result(std::string path, std::string *header, int *Nlines, int *Ncolumns);/* 結果を単純に2次元配列に格納する */
	void save_fitting_result(std::string path, std::string header, double** data, double* fitted);
	/* ---- */
	
	/* ---- leastsquare.cpp ---- */
	class LSE_Param{
	private:
	public:
		double **data;
		double offset;
		int number_of_iteration;
	};

	double* leastsquare(double** data, int Nlines);/* data = [i, x, y]. y will be fitted to x. y' = ay + b の 係数 a, b を返す. */
	double log_square_error(const std::vector <double> &Coef, std::vector <double> &grad, void* raw_lsep);//LSE_Param* lsep);
	/* ---- */

}
#endif
