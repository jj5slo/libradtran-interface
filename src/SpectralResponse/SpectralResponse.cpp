/*
 *
 *
 *
 *
 */
#include "SpectralResponse.h"


SpectralResponseWeights& SpectralResponseWeights::operator=(const SpectralResponseWeights& obj){
	if(this == &obj){
		return *this;
	}
	delete[] p_wavelengths;
	delete[] p_weights;

	p_n = obj.p_n;
	p_wavelengths = new double[p_n];
	p_weights     = new double[p_n];
	for(int i=0; i<obj.p_n; i++){
		p_wavelengths[i] = obj.p_wavelengths[i];
		p_weights[i] = obj.p_weights[i];
	}

	return *this;
}
	
void SpectralResponseWeights::init(int n){
	p_n           = n;
	p_wavelengths = new double[n];
	p_weights     = new double[n];
	for(int i=0; i<n; i++){
		p_wavelengths[i] = 0.0;
		p_weights[i] = 0.0;
	}
}
SpectralResponseWeights::SpectralResponseWeights(void){
	init(1);
}
SpectralResponseWeights::SpectralResponseWeights(const SpectralResponseWeights &obj){
	p_n           = obj.p_n;
	p_wavelengths = new double[obj.p_n];
	p_weights     = new double[obj.p_n];
	for(int i=0; i<obj.p_n; i++){
		p_wavelengths[i] = obj.p_wavelengths[i];
		p_weights[i] = obj.p_weights[i];
	}
}
SpectralResponseWeights::SpectralResponseWeights(int n){
	init(n);
}
SpectralResponseWeights::SpectralResponseWeights(int n, double* wavelengths, double* weights){
	p_n           = n;
	p_wavelengths = new double[n];
	p_weights     = new double[n];
	for(int i=0; i<n; i++){
		p_wavelengths[i] = wavelengths[i];
		p_weights[i]     = weights[i];
	}
}
SpectralResponseWeights::~SpectralResponseWeights(void){
//	delete   &p_n;
	delete[] p_wavelengths;
	delete[] p_weights;
}

//void SpectralResponseWeights::reset(int n, double* wavelengths, double* weights){
//	if(n != p_n){
//		throw std::runtime_error("number of wavelengths does not match!");
//	}
//	p_wavelengths = wavelengths;
//	p_weights     = weights;
//	return;
//}
//void SpectralResponseWeights::set(int index, double wavelength, double weight){
//	if(index >= p_n){
//		throw std::runtime_error("number of wavelengths does not match!");
//	}
//	p_wavelengths[index] = wavelength;
//	p_weights[index]     = weight;
//	return;
//}


int SpectralResponseWeights::N(void){
	return p_n;
}

double SpectralResponseWeights::wavelength(int i){
	return p_wavelengths[i];
}

double SpectralResponseWeights::weight(int i){
	return p_weights[i];
}

double SpectralResponseWeights::sum_weights(void){
	double sum = 0.0;
	for(int i=0; i<p_n; i++){
		sum += p_weights[i];
	}
	return sum;
}

void SpectralResponseWeights::save(std::string path){
	double** data = new double*[2];
	data[0] = p_wavelengths;
	data[1] = p_weights;

	readwrite::save_data(path, "#wavelength weight", p_n, 2, data);

	//中身は消したら駄目
	delete[] data;
}


SpectralResponseWeights read_SRWeights(std::string path){
	int Ncolumns;
	int Nlines;
	std::string header;

	double** data = readwrite::read_data(path, header, Nlines, Ncolumns);
	if(Ncolumns < 2){ throw std::runtime_error("read_SRWeights: There is no weight column!"); }	

	SpectralResponseWeights sr(Nlines, data[0], data[1]);
	
	std::cout << "read_SRWeights: done." << std::endl;

	AndoLab::deallocate_memory2d(data);

	return sr;
}
