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
	
	double* fitting_result(int Ndata, double* height, double* obs, double* sim, double min_height, double max_height, double offset, int TYPE);/* 観測(フィッティング先)データ, シミュレーション(被フィッティング)データ, フィッティングに使用する最小高度, 最大高度, オフセット, フィッティングの種類 */
	
//	double* fitting_result(std::string path, double min_height, double max_height, double offset, int TYPE, int* Ndata); /* saveで作ったファイルから読み込む. ファイルに保存もする */
	/* ---- */

	/* ---- apply_fitting.cpp ---- */
double* apply_fitting(int Ndata, double* data, double* a_offset);
	/* ---- */

	/* ---- fit_readwrite.cpp ---- */
	double** read_result(std::string path, std::string &header, int &Nlines, int &Ncolumns);/* 読んだ結果を単純に2次元配列に格納する */
	void save_data_and_result(std::string path, std::string header, int Nlines, double* data, double* result);
	void save_fitting_result(std::string path, std::string header, int Nlines,  double** data, double* fitted);
	void save_data_and_result(std::string path, std::string header, int Nlines, int Ncolumns, double** data, double* result);/* dataにresultの列を追加して保存 */
	void save_data_and_result(std::string path, std::string header, int Nlines, int Ncolumns, double** data, int Nresult_columns, double** result);/* dataにresultの列(複数)を追加して保存 */
	void save_data(std::string path, std::string header, int Nlines, int Ncolumns, double** data);/* dataを保存 */
	/* ---- */
	/* ---- mean.cpp ---- */
	double mean(std::string path, double min_height, double max_height);/* 観測値の指定高度範囲の平均 */
	double mean(int Ndata, double**data, double min_height, double max_height);
	double mean(int Ndata, double* data);
	/* ---- */
	/* ---- leastsquare.cpp ---- */
	class SLE_Param{
	private:
	public:
		double **data;
		double offset;
		int Ndata;/* データの数 */
		int number_of_iteration;/* 繰り返し回数 */

		double min_height;
		double max_height;
	};

	double* leastsquare(double** data, int Nlines, double min_height, double max_height);/* data = [i, x, y]. y will be fitted to x. y' = ay + b の 係数 a, b を返す. */
	double square_log_error(const std::vector <double> &Coef, std::vector <double> &grad, void* raw_lsep);//SLE_Param* lsep);
	/* ---- */
	/* ---- running_mean.cpp ---- */
	double* running_mean(int Nlines, int Nmean, double* data);
	/* ---- */

}
#endif
