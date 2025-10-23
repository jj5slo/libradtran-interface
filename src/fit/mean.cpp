#include "fit.h"

double fit::mean(std::string path, double min_height, double max_height){
	int Nlines;
	int Ncolumns;
	std::string header;
	double** data = fit::read_result( path, header, Nlines, Ncolumns );
	return fit::mean(data, Nlines, min_height, max_height);
}

double fit::mean(double**data, int Ndata, double min_height, double max_height){
	double sum = 0.0;
	int count = 0;
	for(int i=0; i<Ndata; i++){
		if( min_height <= data[0][i] && data[0][i] <= max_height ){
			sum += data[1][i];/* 観測値 */
			count++;
		}
	}
	return sum / (double)count;
}
	
