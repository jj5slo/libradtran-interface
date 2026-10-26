/*
 * atmosphere_edit.cpp
 * 大気ファイルを読み込んで編集する
 * ある高度以下を指数にする
 */



#include <iostream>
#include "interface.h"
#include "atmNair_to_temperature.h"
#include "ylog_interpolation.h"


int main(int argc, char *argv[]){
	/* ==== 引数処理 ==== */
	if(argc != 3){
		std::cerr << "Usage: ./save_101km_atm INPUT OUTPUT" << std::endl;
		return 1;
	}
	const std::string INPUT  = std::string(argv[1]);
	const std::string OUTPUT = std::string(argv[2]);

	std::cout << INPUT <<" "<< OUTPUT <<" "<< std::endl;

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
	double* z    = new double[Nheights];
	double* p    = new double[Nheights];
	double* T    = new double[Nheights];
	double* Nair = new double[Nheights];
	double* No3  = new double[Nheights];
	double* No2  = new double[Nheights];
	double* Nh2o = new double[Nheights];
	double* Nco2 = new double[Nheights];
	double* Nno2 = new double[Nheights];
	for(int i=0; i<Nheights; ++i){
		z[i]    = atm[i].z;
		p[i]    = atm[i].p;
		T[i]    = atm[i].T;
		Nair[i] = atm[i].Nair;
		No3[i]  = atm[i].No3;
		No2[i]  = atm[i].No2;
		Nh2o[i] = atm[i].Nh2o;
		Nco2[i] = atm[i].Nco2;
		Nno2[i] = atm[i].Nno2;
	}
	ParamAtmosphere* newatm = new ParamAtmosphere[101];
	for( int i=0; i<=100; ++i){
		newatm[i].z    = 1.0*(double)i;
		newatm[i].p    = ylog_interpolation(z, p, Nheights, newatm[i].z);
		newatm[i].T    = ylog_interpolation(z, T, Nheights, newatm[i].z);
		newatm[i].Nair = ylog_interpolation(z, Nair, Nheights, newatm[i].z);
		newatm[i].No3  = ylog_interpolation(z, No3, Nheights, newatm[i].z);
		newatm[i].No2  = ylog_interpolation(z, No2, Nheights, newatm[i].z);
		newatm[i].Nh2o = ylog_interpolation(z, Nh2o, Nheights, newatm[i].z);
		newatm[i].Nco2 = ylog_interpolation(z, Nco2, Nheights, newatm[i].z);
		newatm[i].Nno2 = ylog_interpolation(z, Nno2, Nheights, newatm[i].z);
	}
	
	saveParamAtmosphere(OUTPUT, newatm, 101, 7);
	

	delete[] atm;
	delete[] newatm;
	delete[] z;
	delete[] p   ;
	delete[] T   ;
	delete[] Nair;
	delete[] No3 ;
	delete[] No2 ;
	delete[] Nh2o;
	delete[] Nco2;
	delete[] Nno2;
	return 0;
}


