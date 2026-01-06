#include <cmath>
#include <vector>
#include <iostream>

#include "golden_section_search.h"

constexpr double inv_GOLDEN_RATIO { 2.0 / (1.0 + std::sqrt(5.0)) };/* = b/(a+b) */
constexpr double a_by_r { 2.0 / (3.0 + std::sqrt(5.0) ) };

/* MINIMIZATION */
double golden_section_search( double& x, double x_min, double x_max, double xtol, double (*wrapper)(const std::vector<double>&, std::vector<double>&, void*), void* args){
	std::vector<double> grad;
	double x_1 = x_min;
	double x_2 = x_max;
	double x_3;
	double x_4;
	double x_range = x_2 - x_1;
	double f_3;
	double f_4;
	double minf;
	int flag_calc_f_3 = 1;
	int flag_calc_f_4 = 1;
	int iteration_limit = int(std::ceil(std::log(xtol / std::abs(x_range)) / std::log(inv_GOLDEN_RATIO) ) *2 );

	for(int i=0; i<iteration_limit ; i++){
		if(std::abs(x_range) <= xtol){
			std::cerr << "golden_section_search: xtol reached." << std::endl;
			break;
		}
		double xb = x_range * inv_GOLDEN_RATIO;
		if(flag_calc_f_3){
			x_3 = x_2 - xb;
			f_3 = wrapper(std::vector<double>(1, x_3), grad, args); 
//			std::cerr <<"3: "<< x_3 <<" "<< f_3 << std::endl;
		}
		if(flag_calc_f_4){
			x_4 = x_1 + xb;
			f_4 = wrapper(std::vector<double>(1, x_4), grad, args); 
//			std::cerr <<"4: "<< x_4 <<" "<< f_4 << std::endl;
		}
		if(f_3 < f_4){
			minf = f_3;
			x_2 = x_4;
			x_4 = x_3;
			f_4 = f_3;
			flag_calc_f_3 = 1;
			flag_calc_f_4 = 0;
//			std::cerr << "f3" << std::endl;
		}
		else if(f_4 < f_3){
			minf = f_4;
			x_1 = x_3;
			x_3 = x_4;
			f_3 = f_4;
			flag_calc_f_3 = 0;
			flag_calc_f_4 = 1;
//			std::cerr << "f4" << std::endl;
		}
		else if(f_3 == f_4){
		/* 何もせずもう一度 */
//			fmin = f_3;
//			break;
		}
		x_range = x_2 - x_1;
	}
	x = x_3;
	return minf;
}

