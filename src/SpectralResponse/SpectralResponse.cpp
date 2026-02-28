/*
 *
 *
 *
 *
 */
#include "SpectralResponse.h"


void SpectralResponseWeights::init(int n){
	p_n           = n;
	p_wavelengths = new double[n];
	p_weights     = new double[n];
}
SpectralResponseWeights::SpectralResponseWeights(void){
	init(1);
}
SpectralResponseWeights::SpectralResponseWeights(int n){
	init(n);
}
SpectralResponseWeights::SpectralResponseWeights(int n, double* wavelengths, double* weights){
	init(n);
	reset(n, wavelengths, weights);
}
SpectralResponseWeights::~SpectralResponseWeights(void){
//	delete   &p_n;
	delete[] p_wavelengths;
	delete[] p_weights;
}

void SpectralResponseWeights::reset(int n, double* wavelengths, double* weights){
	if(n != p_n){
		throw std::runtime_error("number of wavelengths does not match!");
	}
	p_wavelengths = wavelengths;
	p_weights     = weights;
	return;
}
void SpectralResponseWeights::set(int index, double wavelength, double weight){
	if(index >= p_n){
		throw std::runtime_error("number of wavelengths does not match!");
	}
	p_wavelengths[index] = wavelength;
	p_weights[index]     = weight;
	return;
}



SpectralResponseWeights read_SRWeights(std::string path, int& Nwavelengths){
	int Ncolumns;
	std::string header;

	double** data = readwrite::read_data(path, header, Nwavelengths, Ncolumns);
	if(Ncolumns < 2){ throw std::runtime_error("read_SRWeights: There is no weight column!"); }	

	SpectralResponseWeights sr(Nlines, data[0], data[1]);

	return sr;
}
