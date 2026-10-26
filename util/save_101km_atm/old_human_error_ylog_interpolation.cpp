#include "linear_interpolation.h"

double linear_interpolation(double** data, int N, double x){
	/* data[2][N]
	   x y
	   x y
		 no dupe x assumed.
	 */
	
	int it;

	it = 0;
	int    x_minus_i = -1; 
	double x_minus  ; 
	double xd_minus ;
	for(int i=0; i<N; i++){
		double xd = x - data[0][i];
		if( (0 < xd) ){
			x_minus_i = i;
			x_minus   = data[0][x_minus_i];
			xd_minus  = x - x_minus; 
			it = i;
			break;
		}
	}
	for(int i=it; i<N; i++){
		double xd = x - data[0][i];
		if( (0 < xd) && (xd < xd_minus) ){
			x_minus_i = i;
			x_minus   = data[0][x_minus_i];
			xd_minus  = x - x_minus; 
		}
	}

	it = 0;
	int    x_plus_i  = -1;
	double x_plus   ; 
	double xd_plus  ; 

	for(int i=0; i<N; i++){
		double xd = x - data[0][i];
		if( (xd < 0) && (xd_plus < xd) ){
			x_plus_i = i;
			x_plus    = data[0][x_plus_i];
			xd_plus  = x - x_plus; 
		}
	}
	for(int i=it; i<N; i++){
		double xd = x - data[0][i];
		if( (xd < 0) && (xd_plus < xd) ){
			x_plus_i = i;
			x_plus    = data[0][x_plus_i];
			xd_plus  = x - x_plus; 
		}
	}
	
	if( x_minus_i == -1 && x_plus_i == -1 ){
		std::cerr << "linear_interpolation: ERROR!" << std::endl;
	}
	if( x_minus_i == -1 ){ return data[1][x_plus_i] ; }
	if( x_plus_i  == -1 ){ return data[1][x_minus_i]; }
	if( x_minus_i == x_plus_i ){
		return data[1][x_minus_i];
	}

	std::cout << x_plus << std::endl;
	std::cout << x_minus << std::endl;
		
	double y_minus = data[1][x_minus_i];
	double y_plus  = data[1][x_plus_i];

	return y_minus + (y_plus-y_minus)/(x_plus-x_minus) * (x-x_minus);
}

