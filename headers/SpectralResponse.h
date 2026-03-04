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
	SpectralResponseWeights& operator=(const SpectralResponseWeights& obj);
	void init(int n);
	SpectralResponseWeights(void); 
	SpectralResponseWeights(const SpectralResponseWeights &obj);
	SpectralResponseWeights(int n); 
	SpectralResponseWeights(int n, double* wavelengths, double* weights); 
	~SpectralResponseWeights(void);
//	void reset(int n, double* wavelengths, double* weights);
//	void set(int index, double wavelength, double weight);

	int N(void);
	double wavelength(int i);
	double weight(int i);
	double sum_weights(void);

	void save(std::string path);
};

SpectralResponseWeights read_SRWeights(
	std::string path
	//int& Nwavelengths
	);
