/* 
 * radtran-interface-atmosphere.cpp
 *
 * libRadtranの大気プロファイル設定ファイルを作る
 *
 */

#include<iostream>
#include<fstream>
#include<iomanip>

#include<memory_allocate.h>

#include"interface.h"

//constexpr int PREC {6};

void saveParamAtmosphere(std::string filename, ParamAtmosphere *params, int Nlines, int Ndecimal){/* ParamAtmosphere からファイルを作る関数 */
	int NoPs = params[0].NoPs();
	double** data = AndoLab::allocate_memory2d(Nlines, NoPs, 0.0);
	for(int i=0; i<Nlines; i++){
		data[i] = params[i].returnvector();
	}
//	std::ostringstream oss; buffer
	std::ofstream ofs(filename);
	if(!ofs.is_open()){
		std::cerr<< "radtran-interface::saveParamAtmosphere: file could not open.\n";
	}
	ofs << "# z[km] p[hPa] T[K] Nair[cm-3] No3[cm-3] No2[cm-3] Nh2o[cm-3] Nco2[cm-3] Nno2[cm-3]\n";
	ofs << std::setprecision(Ndecimal) << std::scientific;
	for(int i=0; i<Nlines; i++){
		for(int j=0; j<NoPs; j++){
			ofs << data[Nlines-1-i][j] << " ";
		}
		ofs << "\n";
	}
	ofs.close();
	AndoLab::deallocate_memory2d(data);
}

	
 

double* ParamAtmosphere::returnvector(void){
	double *linevector = new double [p_NoPs];
	linevector[0] = z;
	linevector[1] = p;
	linevector[2] = T;
	linevector[3] = Nair;
	linevector[4] = No3;
	linevector[5] = No2;
	linevector[6] = Nh2o;
	linevector[7] = Nco2;
	linevector[8] = Nno2;
	return linevector;
}
void ParamAtmosphere::set_p_from_Nair_T(void){
	p = (Nair*1e6) * BOLTZMANN_CONSTANT * T * 1e-2;
}




