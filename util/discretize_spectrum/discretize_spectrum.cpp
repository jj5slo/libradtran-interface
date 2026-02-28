/*
 * discretize_spectrum.cpp
 *
 *
 */
//元のスペクトルをあたえる
//分割する範囲を指定
//何分割するか
//それぞれの代表値を決定
//重みを決定

#include "discretize_spectrum.h"

int main(int argc, char *argv[]){
	std::string path;
	double MIN_WL;
	double MAX_WL;
	int NUMBER_OF_PART;
	if (argc == 5){
		path           = std::string(argv[1]);
		MIN_WL         = std::stod(argv[2]);
		MAX_WL         = std::stod(argv[3]);
		NUMBER_OF_PART = std::stod(argv[4]);
	}
	else{
		std::cerr << "Usage: ./discretize_spectrum PATH_SPECTRUM MIN_WL MAX_WL NUMBER_OF_PART" << std::endl;
		return 1;
	}
	std::string header;
	int Nlines;
	int Ncolumns;
	double** spectrum = readwrite::read_data(path, header, Nlines, Ncolumns);
	
	Srf* srf             = new Srf[Nlines];
	for(int i=0; i < Nlines; i++){
		srf[i].wavelength = spectrum[0][i];
		srf[i].response	 = spectrum[2][i];
	}
	Srf* discretized_srf = new Srf[NUMBER_OF_PART];
	
	double WL_RANGE = MAX_WL - MIN_WL;
	double WL_UNIT  = WL_RANGE / NUMBER_OF_PART;
	
	for (int i=0; i < NUMBER_OF_PART; i++){
		double wl_start = MIN_WL + i*WL_UNIT;
		double wl_end   = wl_start + WL_UNIT;

		discretized_srf[i].wavelength = (wl_start + wl_end) / 2.0;
		discretized_srf[i].response   = integrateSrf(Nlines, srf, wl_start, wl_end);
	}

	double** ret = new double*[2];
	ret[0] = new double[NUMBER_OF_PART];/* wavelength */
	ret[1] = new double[NUMBER_OF_PART];/* weight */
	for(int i=0; i < NUMBER_OF_PART; i++){
		ret[0][i] = discretized_srf[i].wavelength;
		ret[1][i] = discretized_srf[i].response;
	}
	
	header = "#"+path+"\n#wavelength weight";
	path   = "srf_weight.dat";
	readwrite::save_data(path, header, NUMBER_OF_PART, 2, ret);
	std::cout << "Saved: " << path << std::endl;

	return 0;
}



// 指定された波長区間 [lambda_start, lambda_end] のResponseを台形公式で積分する関数
// 参照渡し(const std::vector&)にすることでコピーのオーバーヘッドを無くし高速化しています
// 境界を厳密に考慮した台形積分
double integrateSrf(int Nsrf, Srf*& srf, double lambda_start, double lambda_end) {
	double weight = 0.0;

	for (int i = 0; i < Nsrf - 1; i++) {
		double x0 = srf[i].wavelength;
		double x1 = srf[i+1].wavelength;
		double y0 = srf[i].response;
		double y1 = srf[i+1].response;

		// データ区間 [x0, x1] と 積分区間 [lambda_start, lambda_end] の重なりを求める
		double overlap_start = std::max(x0, lambda_start);
		double overlap_end = std::min(x1, lambda_end);

		// 重なりがある場合のみ計算する
		if (overlap_start < overlap_end) {
				
			// 補間関数 (C++11のラムダ式を使ってすっきり記述)
			auto interpolate = [&](double target_x) {
				if (target_x == x0) return y0;
				if (target_x == x1) return y1;
				return y0 + (y1 - y0) * (target_x - x0) / (x1 - x0);
			};

			// 重なり区間の両端におけるResponseを計算（線形補間）
			double y_start = interpolate(overlap_start);
			double y_end = interpolate(overlap_end);

			// 台形の面積を足し込む
			weight += (y_start + y_end) / 2.0 * (overlap_end - overlap_start);
		}
	}
	return weight;
}
