#include <iostream>

#include "interface.h"
#include "read_config.h"

int main(int argc, char *argv[]){
	
	std::string PATH_CONFIG = "./config.conf";
	std::map<std::string, std::string> configs = readConfigFile(PATH_CONFIG);

	std::string PATH_ATMOSPHERE = getConfig(configs, "PATH_ATMOSPHERE", std::string(std::getenv("HOME"))+"/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/data/atmmod/afglus.dat");/* libRadtranに渡す大気ファイル */
	std::string PATH_ATMOSPHERE_INIT = getConfig(configs, "PATH_ATMOSPHERE_INIT", std::string(std::getenv("HOME"))+"/SANO/research/LIBRARIES/libradtran/libRadtran-2.0.6/data/atmmod/afglus.dat");/* libRadtranに渡す大気ファイル */
	
	int i_top = getConfig(configs, "i_top", 64);/* 数密度を求める最高高度（index） */
	int i_bottom = getConfig(configs, "i_bottom", 60);/* 数密度を求める最低高度（index） */
	int atmosphere_precision = getConfig(configs, "atmosphere_precision", 7);/* 保存する大気の保存時の精度 */

	double coef = getConfig(configs, "coef", 1.0);/* 数密度の変更係数 */

	int Nheights;

	ParamAtmosphere* pAtm = readParamAtmosphere(PATH_ATMOSPHERE_INIT, Nheights);
	std::cout << "# Init\nz Nair p T" << std::endl;
	for(int i=Nheights-1; 0<=i; i--){
		std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
	}

/* ==== MODIFY HERE ==== */

	for(int i=i_bottom; i<=i_top; i++){/* TODO NOW MSISから考えている高度範囲だけはずらした上で最適化で戻るかどうか */
		pAtm[i].Nair = pAtm[i].Nair * coef;
		pAtm[i].set_p_from_Nair_T();
	}

/* ==== */



	std::cout << "# Modified Atmosphere\nz Nair p T" << std::endl;
	for(int i=Nheights-1; 0<=i; i--){
		std::cout << " " << pAtm[i].z << " " << pAtm[i].Nair << " " << pAtm[i].p << " " <<  pAtm[i].T << std::endl;
	}
	
	saveParamAtmosphere(PATH_ATMOSPHERE, pAtm, Nheights, atmosphere_precision);

	return 0;
}

