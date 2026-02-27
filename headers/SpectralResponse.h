/*
 *
 *
 *
 *
 */
#include <iostream>
#include <string>
//#include <exception>


#include "readwrite.h"

class SpectralResponse{
private:
	int     p_n;
	double* p_wavelengths;
	double* p_weights;
	
public:
	void init(int n);
	SpectralResponse(void); 
	SpectralResponse(int n); 
	SpectralResponse(int n, double* wavelengths, double* weights); 
	~SpectralResponse(void);
	void set(int n, double* wavelengths, double* weights);

	int number_of(void);
	double weight(int i);
	double wavelength(int i);
};

SpectralResponse read_SpectralResponse(
	std::string path,
	int column_wavelength_start_from_0,
	int column_response_start_from_0
	);
