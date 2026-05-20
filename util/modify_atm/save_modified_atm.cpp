#include <iostream>

#include "interface.h"

int main(int argc, char *argv[]){
	
	std::string PATH_ATMOSPHERE_INIT = "original.dat";
	std::string PATH_ATMOSPHERE      = "modified.dat";
	
	int atmosphere_precision = 7;/* 保存する大気の保存時の精度 */

	int Nheights;

	ParamAtmosphere* pAtm = readParamAtmosphere(PATH_ATMOSPHERE_INIT, Nheights);
	std::cout << "# Init\nz Nair p T" << std::endl;
	for(int i=Nheights-1; 0<=i; i--){
		std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
	}

/* ==== MODIFY HERE ==== */

	for(int i=0; i<Nheights; i++){
		pAtm[i].Nair = 1.0e11;
		pAtm[i].set_T_from_Nair_p();
	}

/* ==== */



	std::cout << "# Modified Atmosphere\nz Nair p T" << std::endl;
	for(int i=Nheights-1; 0<=i; i--){
		std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
	}
	
	saveParamAtmosphere(PATH_ATMOSPHERE, pAtm, Nheights, atmosphere_precision);

	return 0;
}

