#include "fit.h"

int main(int argc, char* argv[]){
	if(argc != 2){
		std::cerr << "Usage ./fit path" << std::endl;
		return 1;
	}
	std::string path = argv[1];
	std::cerr << path << std::endl;
	int Ndata;
	double offset = fit::mean(path, 95.0, 100.0);
	std::cout << offset << std::endl;
	double* fitted = fit::fitting_result(path, 20.0, 60.0, offset, fit::LOG, &Ndata);
	for(int i = 0; i<Ndata; i++){
		std::cout << fitted[i] << " ";
	}
	std::cout << std::endl;
}
