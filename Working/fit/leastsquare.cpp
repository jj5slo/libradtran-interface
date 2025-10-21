#include"fit.h"



double* fit::leastsquare(double** data, int Nlines){
	/*  
			data[1] : x(observed, fit to)
			data[2] : y(simulated, to fit)
	*/
	double* fitting_coefficient = new double[2];

	double sum_x;
	double sum_y;
	double sum_y2;
	double sum_xy;
	/* y' = ay + b */
	double a{0.0};
	double b{0.0};

	for(int i = 0; i < Nlines; i++){
//		if(h[i] >= 20.0){
			sum_xy += data[1][i] * data[2][i];
			sum_x += data[1][i];
			sum_y += data[2][i];
			sum_y2 += data[2][i]*data[2][i];
//		}
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

double fit::log_square_error(const std::vector <double> &Coef, std::vector <double> &grad, void* raw_lsep){// double **data, double offset, int &number_of_iteration){
	fit::LSE_Param* lsep = static_cast<fit::LSE_Param*>(raw_lsep);
	double err = 0.0;
	double grad_err = 0.0;
	double logdiff = 0.0;
	double a = 0.0;
	for(int j=0; j<Ndata; j++){/* 対数の二乗誤差をとる */
		logdiff = std::log10(lsep->data[1][j]) - std::log10(a*lsep->data[2][j]+lsep->offset);
		err += logdiff*logdiff;
		grad_err += (-2.0) * logdiff * lsep->data[2][j] / (a*lsep->data[2][j]+lsep->offset);
	}
	lsep->number_of_iteration++;
	if( number_of_iteration > 10000 ){
		throw nlopt::forced_stop();
	}
}
