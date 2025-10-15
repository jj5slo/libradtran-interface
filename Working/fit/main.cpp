#include "fit.h"

int main(int argc, char* argv[]){
	if(argc != 2){
		std::cerr << "Usage ./fit path" << std::endl;
	}
	std::string path = std::string(argv[1]);
	int Ndata;
	double* fitted = fitting_result(path, LOG_LS, &Ndata);
	for(int i = 0; i<Ndata; i++){
		std::cout << fitted[i] << " ";
	}
	std::cout << std::endl;
}
