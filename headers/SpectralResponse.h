/*
 * ラインバイライン。
 *
 *
 *
 */
#include <iostream>
#include <string>
//#include <exception>


#include "readwrite.h"

class SpectralResponseWeights{
private:
	int     p_n;
	double* p_wavelengths;
	double* p_weights;
	
public:
	void init(int n);
	SpectralResponseWeights(void); 
	SpectralResponseWeights(int n); 
	SpectralResponseWeights(int n, double* wavelengths, double* weights); 
	~SpectralResponseWeights(void);
	void reset(int n, double* wavelengths, double* weights);
	void set(int index, double wavelength, double weight);

	int number_of(void);
	double weight(int i);
	double wavelength(int i);
};

SpectralResponseWeights read_SRWeights(
	std::string path,
	int& Nwavelengths
	);
