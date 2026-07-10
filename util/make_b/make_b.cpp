#include <string>

#include "readwrite.h"


int main(int argc, char* argv[]){
	std::string PATH = "";
	if(argc == 2){
		PATH = std::string(argv[1]);
	}else{
		std::cerr << "Usage: ./this PATH" << std::endl;
		return 1;
	}
	std::string header;
	int Nlines;
	int Ncolumns;

	double** data = readwrite::read_data(PATH, header, Nlines, Ncolumns);
	double** ret = new double* [2];
	ret[0] = new double [101];
	ret[1] = new double [101];

	for(int i=0; i<(101-Nlines)/2; ++i){
		ret[0][i] = (double)i;
		ret[1][i] = 0.0;
		std::cout << ret[0][i] << " " << ret[1][i] << "(nan)" << std::endl;
	}
	for(int i=0; i<Nlines; ++i){
		ret[0][i+(101-Nlines)/2] = (data[0][i] + data[1][i]) / 2.0;
		ret[1][i+(101-Nlines)/2] = (data[3][i]);
		std::cout << ret[0][i+(101-Nlines)/2] << " " << ret[1][i+(101-Nlines)/2] << std::endl;
	}
	for(int i=Nlines+(101-Nlines)/2; i<101; ++i){
		ret[0][i] = (double)i;
		ret[1][i] = 0.0;
		std::cout << ret[0][i] << " " << ret[1][i] << "(nan)" << std::endl;
	}

	readwrite::save_data(PATH+"_b.dat", header, 101, 2, ret);

	delete[] ret[0];
	delete[] ret[1];
	delete[] ret;

	AndoLab::deallocate_memory2d(data);
	return 0;
}
