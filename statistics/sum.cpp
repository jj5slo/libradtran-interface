#include"statistics.h"

double sum(int N, double* data){
	double summation{0.0};
	for(int i=0; i<N; i++){
		summation += data[i];
	}
	return summation;
}

double sum_square(int N, double* data){
	double summation{0.0};
	for(int i=0; i<N; i++){
		summation += data[i]*data[i];
	}
	return summation;
}

double 
