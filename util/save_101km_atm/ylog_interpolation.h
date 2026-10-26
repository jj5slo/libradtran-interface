#include <iostream>

double ylog_interpolation(const double** data, const int N, const double x);

double ylog_interpolation(
	const double* xdata,
	const double* ydata,
	const int N,
	const double x
){
	const double** data = new const double* [2];
	data[0] = xdata;
	data[1] = ydata;
	return ylog_interpolation(data, N, x);
}
