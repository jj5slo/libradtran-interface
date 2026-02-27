/*
 *
 *
 *
 *
 */
#include "SpectralResponse.h"

void SpectralResponse::init(int n){
	p_n           = n;
	p_wavelengths = new double[n];
	p_weights     = new double[n];
}
SpectralResponse::SpectralResponse(void){
	init(1);
}
SpectralResponse::SpectralResponse(int n){
	init(n);
}
SpectralResponse::SpectralResponse(int n, double* wavelengths, double* weights){
	init(n);
	set(n, wavelengths, weights);
}
SpectralResponse::~SpectralResponse(void){
//	delete   &p_n;
	delete[] p_wavelengths;
	delete[] p_weights;
}

void SpectralResponse::set(int n, double* wavelengths, double* weights){
	if(n != p_n){
		throw std::runtime_error("number of wavelengths does not match!");
	}
	p_wavelengths = wavelengths;
	p_weights     = weights;
	return;
}



SpectralResponse read_SpectralResponse(std::string path, int column_wavelength, int column_response){
	int Nlines;
	int Ncolumns;
	std::string header;

	double** data = readwrite::read_data(path, header, Nlines, Ncolumns);
	
	SpectralResponse sr(Nlines, data[column_wavelength], data[column_response]);
	return sr;
}
