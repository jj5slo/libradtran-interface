#include <iostream>
#include <cmath>
#include "readwrite.h"
#include "linear_regression.h"

double* calc_b(int heightwidth, double* height, double* I_obs, double* I_sim){
	double B_from_55and60 = (I_sim[0]*I_obs[heightwidth-1] - I_sim[heightwidth-1]*I_obs[0]) / (I_sim[0] - I_sim[heightwidth-1]);
//	std::cout << "B_from_55and60: " << B_from_55and60 << std::endl;

	double* obs_regression = log_regression(height, I_obs, heightwidth);
	double* sim_regression = log_regression(height, I_sim, heightwidth);
	
	double I_obs_reg_55 = std::pow(10, obs_regression[0]*height[0]             + obs_regression[1]);
	double I_obs_reg_60 = std::pow(10, obs_regression[0]*height[heightwidth-1] + obs_regression[1]);
	double I_sim_reg_55 = std::pow(10, sim_regression[0]*height[0]             + sim_regression[1]);
	double I_sim_reg_60 = std::pow(10, sim_regression[0]*height[heightwidth-1] + sim_regression[1]);

//	std::cout << "I_obs_regression: " << obs_regression[0] <<" "<<obs_regression[1] << std::endl;
//	std::cout << "minh: " << I_obs_reg_55 << ", maxh: " << I_obs_reg_60 << std::endl;
//	std::cout << "I_sim_regression: " << sim_regression[0] <<" "<<sim_regression[1] << std::endl;
//	std::cout << "minh: " << I_sim_reg_55 << ", maxh: " << I_sim_reg_60 << std::endl;

	double B_from_regression = (I_sim_reg_55*I_obs_reg_60 - I_sim_reg_60*I_obs_reg_55) / (I_sim_reg_55 - I_sim_reg_60);
	
//	std::cout << "B_from_regression: " << B_from_regression << std::endl;
	double* ret = new double[2];
	ret[0] = B_from_55and60;
	ret[1] = B_from_regression;
	
	delete[] obs_regression;
	delete[] sim_regression;
	return ret;
}



int main(int argc, char *argv[]){
	if(argc != 2){ 
		std::cerr << "Usage: calc_B PATH" << std::endl;
		return 1;
	}
	std::string PATH = argv[1];
	std::cout << PATH << std::endl;

//	double minheight = 55.0;
	int heightwidth = 5;

	std::string header;
	int Nlines;
	int Ncolumns;

	double** data = readwrite::read_data(PATH, header, Nlines, Ncolumns);
	std::cout << "h\tobs\tsim" << std::endl;
	for(int i=0; i<Nlines; i++){
		std::cout << data[0][i] <<"\t"<< data[1][i] <<"\t"<< data[3][i] << std::endl;
	}// 3列目、つまりスムージング後の値をシミュレーション値として使用

	int N_result_columns = 4;
	int N_result_lines = 96;
	double** result = new double*[N_result_columns];
	for(int i=0; i<N_result_columns; ++i){
		result[i] = new double[N_result_lines];
	}
	/* lower upper b(direct) b(regression) */
	for(int index = 0; index < N_result_lines; ++index){
		double minheight = (double)index * 1.0 + 0.0;

		/* Initialize */
		double* height = new double[heightwidth];
		double* I_obs =  new double[heightwidth];
		double* I_sim =  new double[heightwidth];
		for(int i=0; i<heightwidth; i++){
			height[i] = 0.0;
			I_obs[i] = 0.0;
			I_sim[i] = 0.0;
		}

		/* Assign */
		for(int i=0; i<Nlines; i++){
			if(std::abs(data[0][i] - minheight) < 0.9){
				for(int j=0; j<heightwidth; j++){
					height[j] = data[0][i+j];
					I_obs[j] = data[1][i+j];
					I_sim[j] = data[3][i+j];
				}
			}
		}
	
		double* b_2 = calc_b(heightwidth, height, I_obs, I_sim);
		result[0][index] = height[0];
		result[1][index] = height[heightwidth - 1];
		result[2][index] = b_2[0];
		result[3][index] = b_2[1];

		delete[] b_2;
		delete[] height;
		delete[] I_obs;
		delete[] I_sim;
	}


/* ==== */
/* ==== */

//	std::cout << "----\nRESULTS" << std::endl;
//	std::cout << "linear:" << std::endl;
//	std::cout << "B_from_55and60: " << B_from_55and60 << std::endl;
//	std::cout << "B_from_regression: " << B_from_regression << std::endl;
//	std::cout << "log10:" << std::endl;
//	std::cout << "B_from_55and60: " << std::pow(10, B_from_55and60_log) << std::endl;
//	std::cout << "B_from_regression: " << std::pow(10, B_from_regression_log) << std::endl;
	
	readwrite::save_data(PATH+"_b.dat", "#heightwidth="+std::to_string(heightwidth)+"\n#lower upper b(direct) b(regression)", N_result_lines, N_result_columns, result);

	AndoLab::deallocate_memory2d(data);
	delete[] result;

	return 0;
}
	
/*
	for(int j=0; j<heightwidth; j++){
		I_obs[j] = std::log10(I_obs[j]);
		I_sim[j] = std::log10(I_sim[j]);
	}
	std::cout << "h\tobs\tsim" << std::endl;
	for(int i=0; i<heightwidth; i++){
		std::cout << height[i] <<"\t"<< I_obs[i] <<"\t"<< I_sim[i] << std::endl;
	}

	double B_from_55and60_log = (I_sim[0]*I_obs[heightwidth-1] - I_sim[heightwidth-1]*I_obs[0]) / (I_sim[0] - I_sim[heightwidth-1]);
	std::cout << "B_from_55and60_log: " << B_from_55and60 << std::endl;

	double* obs_regression_log = linear_regression(height, I_obs, heightwidth);
	double* sim_regression_log = linear_regression(height, I_sim, heightwidth);
	
	double I_obs_reg_55_log = obs_regression_log[0]*height[0]             + obs_regression_log[1];
	double I_obs_reg_60_log = obs_regression_log[0]*height[heightwidth-1] + obs_regression_log[1];
	double I_sim_reg_55_log = sim_regression_log[0]*height[0]             + sim_regression_log[1];
	double I_sim_reg_60_log = sim_regression_log[0]*height[heightwidth-1] + sim_regression_log[1];

	std::cout << "I_obs_regression_log: " << obs_regression_log[0] <<" "<<obs_regression_log[1] << std::endl;
	std::cout << "minh: " << I_obs_reg_55_log << ", maxh: " << I_obs_reg_60_log << std::endl;
	std::cout << "I_sim_regression: " << sim_regression_log[0] <<" "<<sim_regression_log[1] << std::endl;
	std::cout << "minh: " << I_sim_reg_55_log << ", maxh: " << I_sim_reg_60_log << std::endl;

	double B_from_regression_log = (I_sim_reg_55_log*I_obs_reg_60_log - I_sim_reg_60_log*I_obs_reg_55_log) / (I_sim_reg_55_log - I_sim_reg_60_log);
	
	std::cout << "B_from_regression_log: " << B_from_regression_log << std::endl;
*/


