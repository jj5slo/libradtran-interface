#include"fit.h"



double* fit::leastsquare(double** data, int Nlines, double min_height, double max_height){
	/*  
			data[1] : x(observed, fit to)
			data[2] : y(simulated, to fit)
	*/
	double* fitting_coefficient = new double[2];

	double sum_x = 0.0;
	double sum_y = 0.0;
	double sum_y2 = 0.0;
	double sum_xy = 0.0;
	/* y' = ay + b */
	double a{0.0};
	double b{0.0};

	for(int i = 0; i < Nlines; i++){
		if( min_height <= data[0][i] && data[0][i] <= max_height ){
			sum_xy += data[1][i] * data[2][i];
			sum_x += data[1][i];
			sum_y += data[2][i];
			sum_y2 += data[2][i]*data[2][i];
		}	
	}
	double nd = (double)Nlines;/* number of datas */
	a = (nd*sum_xy - (sum_x*sum_y)) / (nd*sum_y2 - (sum_y*sum_y));
	b = (sum_x - sum_y * a)/nd;
	if((nd*sum_y2 - (sum_y*sum_y)) == 0.0){
		a = 1.0;
		b = sum_x / nd;
	}

	std::cout << sum_xy <<" "<< sum_x << " "<<sum_y<<" "<<sum_y2<<std::endl;

	fitting_coefficient[0] = a;
	fitting_coefficient[1] = b;

	return fitting_coefficient;
}
double* fit::leastsquare(double* x, double* y, int min_index, int max_index){
	/*  
			data[1] : x(observed, fit to)
			data[2] : y(simulated, to fit)
	*/
	double* fitting_coefficient = new double[2];

	double sum_x = 0.0;
	double sum_y = 0.0;
	double sum_y2 = 0.0;
	double sum_xy = 0.0;
	/* y' = ay + b */
	double a{0.0};
	double b{0.0};

	for(int i = min_index; i <= max_index; i++){
			sum_xy += x[i] * y[i];
			sum_x += x[i];
			sum_y += y[i];
			sum_y2 += y[i]*y[i];
		}	
	double nd = (double)( max_index - min_index + 1 );/* number of datas */
	a = (nd*sum_xy - (sum_x*sum_y)) / (nd*sum_y2 - (sum_y*sum_y));
	b = (sum_x - sum_y * a)/nd;
	if((nd*sum_y2 - (sum_y*sum_y)) == 0.0){
		a = 1.0;
		b = sum_x / nd;
	}

	std::cout << sum_xy <<" "<< sum_x << " "<<sum_y<<" "<<sum_y2<<std::endl;

	fitting_coefficient[0] = a;
	fitting_coefficient[1] = b;

	return fitting_coefficient;
}

double fit::least_square_log_error(const std::vector <double> &Coef, std::vector <double> &grad, void* raw_slep){// double **data, double offset, int &number_of_iteration){
	fit::SLE_Param* slep = static_cast<fit::SLE_Param*>(raw_slep);
	double err = 0.0;
	double grad_err = 0.0;
	double logdiff = 0.0;
	double a = 0.0;
	a = Coef[0];
//	std::cout << "least_square_log_error: min_index=" << slep->min_index << " max_index=" << slep->max_index << std::endl << "a=" << a << std::endl;
	for(int j=slep->min_index; j <= slep->max_index; j++){/* 対数の二乗誤差をとる */
//		std::cout << slep->x[j] << " " << slep->y[j] << std::endl;
		logdiff = std::log10(slep->x[j]) - std::log10(a * slep->y[j] + slep->offset);
		err += logdiff*logdiff;
		grad_err += (-2.0) * logdiff * slep->y[j] / (a*slep->y[j]+slep->offset);
	}

	slep->number_of_iteration++;
	if( slep->number_of_iteration > 10000 ){
		std::cerr << "least_square_log_error: iteration by nlopt stopped." << std::endl;
		throw nlopt::forced_stop();
	}
	if( !grad.empty() ){/* 微分を使わないアルゴリズムもある */
		grad[0] = grad_err;
	}
//	std::cout << "err:" << err << std::endl;
	return err;
}
