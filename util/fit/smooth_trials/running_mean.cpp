#include "../fit.h"

double* fit::running_mean(int Nlines, int Nmean, double* data){
	int width = Nmean / 2;/* ( Nmean - 1 ) / 2 */
	double* result = new double[Nlines];
	for(int i=0; i<Nlines; i++){
		result[i] = 0.0;/* initialize */
	}
	for(int i=width; i < Nlines - width; i++){
		result[i] += data[i];
		for(int j=1; j<=width; j++){
			result[i] += data[i-j];
			result[i] += data[i+j];
		}
		result[i] = result[i] / (double)Nmean;
	}
	return result;
}

