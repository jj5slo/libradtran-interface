#include <iostream>
#include <string>

#include "interface.h"
#include "save.h"

int main(int argc, char *argv[]){
	constexpr double BOLTZMANN_CONSTANT { 1.380649e-23 };
	std::string path;
	std::string path_to_save;
	int Nheights;
	/* ==== 引数処理 ==== */
	if(argc == 3){
		path = argv[1];
		path_to_save = argv[2];
	}
	else{
		std::cerr << "Usage: ./Nair_to_atm PATH PATH_TO_SAVE" << std::endl;
		return 1;
	}
	ParamAtmosphere* pAtm = readParamAtmosphere(path, Nheights);
	for(int i=0; i<Nheights; i++){
		pAtm[i].T = 273.15;/* ぜろど */
		pAtm[i].p = (pAtm[i].T) * (pAtm[i].Nair * 1.0e6) * BOLTZMANN_CONSTANT / 1.0e2;/* 状態方程式 */
	}
	saveParamAtmosphere(path_to_save, pAtm, Nheights, 6);

	return 0;
}
