/*
 * apply_fitting.cpp
 *
 */

#include "fit.h"

double* fit::apply_fitting(int Ndata, double* data, double* a_offset){
	
	/* ==== フィッティング処理 ==== */
	double* fitted = new double[Ndata];
	if( a_offset[0]<0.0 ){ a_offset[0]=0.0; }/* マイナスはだめでしょう */
	for(int i=0; i<Ndata; i++){
		fitted[i] = a_offset[0]*data[i] + a_offset[1];
	}
	return fitted;
}
