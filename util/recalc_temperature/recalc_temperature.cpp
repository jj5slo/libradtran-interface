/*
 * recalc_temperature.cpp
 * 旧方式（call_temperature -> Nair_to_atmosphere_new）で温度が計算された大気ファイルを読み込み、
 * 新方式（Nair_to_Temperature）で計算し直して保存する
 */



#include <iostream>
#include "interface.h"
#include "atmNair_to_temperature.h"



int main(int argc, char *argv[]){
	/* ==== 引数処理 ==== */
	if(argc != 4){
		std::cerr << "Usage: ./atmosphere_edit INPUT OUTPUT I_TOP(const)" << std::endl;
		return 1;
	}
	const std::string INPUT  = std::string(argv[1]);
	const std::string OUTPUT = std::string(argv[2]);
	const int         I_0    = std::stoi(argv[3]);

	if(INPUT == OUTPUT){
		std::cout << "Warning: INPUT == OUTPUT. stopped for safety!" << std::endl;
		return 1;
	}
	
	std::cout << INPUT <<" -> "<< OUTPUT << std::endl;

	int Nheights = 0;
	ParamAtmosphere* atm = readParamAtmosphere(INPUT, Nheights);// zが小さい順
	PlanetParam earth(6370.e3);
	Nair_to_Temperature(Nheights, atm, I_0, earth);
	
	saveParamAtmosphere(OUTPUT, atm, Nheights, 7);
	
	delete[] atm;
	return 0;
}


