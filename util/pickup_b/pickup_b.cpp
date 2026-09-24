#include <iostream>
#include <cmath>
#include <string>
#include "readwrite.h"


int main(int argc, char *argv[]){
	double LOW = 0.0;
	double HIGH = 0.0;
	std::string PATH = "";
	std::string OUTPUT = "";
	if(argc == 4){
		LOW = std::stod(argv[1]);
		HIGH = std::stod(argv[2]);
		PATH = std::string(argv[3]);
	}
	else if(argc == 6 && std::string(argv[4]) == "-o"){
		LOW = std::stod(argv[1]);
		HIGH = std::stod(argv[2]);
		PATH = std::string(argv[3]);
		OUTPUT = std::string(argv[5]);
	}
	else{
		std::cerr << "Usage: pickup_b LOW HIGH PATH [-o OUTPUT]" << std::endl;
		return 1;
	}
	
	std::string header = "";
	int Nlines = 0;
	int Ncolumns = 0;

	double** data = readwrite::read_data(PATH, header, Nlines, Ncolumns);
	if(Nlines == 0){
		std::cerr << "No data!" << std::endl;
		AndoLab::deallocate_memory2d(data);
		return 1;
	}
	double** b_data = new double* [2];
	for(int i=0; i<2; ++i){
		b_data[i] = new double [101];
	}
	for(int j=0; j<101; ++j){
		b_data[0][j] = 1.0*(double)j;
		b_data[1][j] = 0.0;
	}

	for(int i=0; i<Nlines; ++i){
		if(std::abs(data[0][i] - LOW) < 0.1 && std::abs(data[1][i] - HIGH) < 0.1){
			std::cout << "pickuped " << LOW <<" "<< HIGH << std::endl;
			for(int j=0; j<101; ++j){
				b_data[1][j] = data[3][i];
			}
		}
	}

	if(OUTPUT != ""){
		readwrite::save_data(OUTPUT, "#altitude b\n", 101, 2, b_data);
	}
	else{
		for(int j=0; j<101; ++j){
			std::cout << b_data[0][j] <<" "<< b_data[1][j] << std::endl;
		}
	}

	AndoLab::deallocate_memory2d(data);
	delete[] b_data[0];
	delete[] b_data[1];
	delete[] b_data;

	return 0;
}






