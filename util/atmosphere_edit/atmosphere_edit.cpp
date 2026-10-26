/*
 * atmosphere_edit.cpp
 * 大気ファイルを読み込んで編集する
 * ある高度以下を指数にする
 */



#include <iostream>
#include "interface.h"
#include "atmNair_to_temperature.h"



int main(int argc, char *argv[]){
	/* ==== 引数処理 ==== */
	if(argc != 5){
		std::cerr << "Usage: ./atmosphere_edit INPUT OUTPUT z_0 k_{10}" << std::endl;
		return 1;
	}
	const std::string INPUT  = std::string(argv[1]);
	const std::string OUTPUT = std::string(argv[2]);
	const double I_0         = std::stod(argv[3]);// 指数の基点
	const double K_10        = std::stod(argv[4]);// 指数の係数

	std::cout << INPUT <<" "<< OUTPUT <<" "<< I_0 <<" "<< K_10 << std::endl;

	int Nheights = 0;
	ParamAtmosphere* atm = readParamAtmosphere(INPUT, Nheights);// zが小さい順

//	int iTOP = 0;
//	for(int i=0; i<Nheights; ++i){
//		double z     = rawatm[i].z
//		if(Z_0 < z){
//			iTOP = i-1;
//			break;
//		}
//	}
	set_Nair_10_exponentially(Nheights, atm, I_0, K_10);
	PlanetParam earth(6370.e3);
	Nair_to_Temperature(Nheights, atm, I_0, earth);
	
	saveParamAtmosphere(OUTPUT, atm, Nheights, 7);
	
	delete[] atm;
	return 0;
}


