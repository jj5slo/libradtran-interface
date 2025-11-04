#include "stat.h"

double mean(std::string path){
	int Nlines;
	int Ncolumns;
	std::string header;
	double** data = read_result( path, header, Nlines, Ncolumns );
	return mean(data[1], Nlines);
}

double mean(double*data, int Ndata){
	double sum = 0.0;
	int count = 0;
	for(int i=0; i<Ndata; i++){
		sum += data[1][i];/* 観測値 */
		count++;
	}
	return sum / (double)count;
}


