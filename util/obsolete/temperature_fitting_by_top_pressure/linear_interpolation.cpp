#include <iostream>
#include <cfloat>  // DBL_MAX

#include "linear_interpolation.h"

double linear_interpolation(double** data, int N, double x) {
	int x_minus_i = -1;
	int x_plus_i = -1;
	
	// 距離の最小値を管理するための変数（初期値は最大にしておく）
	double min_dist_minus = DBL_MAX;
	double min_dist_plus  = DBL_MAX;

	for(int i = 0; i < N; i++) {
		double diff = x - data[0][i];

		// Case 0: x がデータ点と完全に一致した場合
		if (diff == 0.0) {
			return data[1][i];
		}

		// Case 1: x より左側 (data[i] < x)
		if ((0 < diff) && (diff < min_dist_minus)) {
			// これまでの候補より x に近ければ更新
			min_dist_minus = diff;
			x_minus_i = i;
		}
		// Case 2: x より右側 (x < data[i])
		else if (-diff < min_dist_plus) {
			// diffは負の値なので、絶対値(-diff)で距離を比較
			min_dist_plus = -diff;
			x_plus_i = i;
		}
	}

	// エラーチェック（両隣が見つからない場合）
	if(x_minus_i == -1 && x_plus_i == -1){
		std::cerr << "linear_interpolation: ERROR! No data points found." << std::endl;
		return 0.0; // エラー時の戻り値（適宜変更してください）
	}

	// 片側しか見つからない場合（外挿はせず、端の値を返す場合）
	if(x_minus_i == -1){ return data[1][x_plus_i] ; }
	if(x_plus_i == -1) { return data[1][x_minus_i]; }

	// デバッグ出力（必要に応じて）
	// std::cout << "L: " << data[0][x_minus_i] << ", R: " << data[0][x_plus_i] << std::endl;

	double x_minus = data[0][x_minus_i];
	double x_plus  = data[0][x_plus_i];
	double y_minus = data[1][x_minus_i];
	double y_plus  = data[1][x_plus_i];

	// 線形補間計算
	// y = y0 + (y1 - y0) * (x - x0) / (x1 - x0)
	return y_minus + (y_plus - y_minus) / (x_plus - x_minus) * (x - x_minus);
}
