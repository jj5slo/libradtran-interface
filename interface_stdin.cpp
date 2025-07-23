/*
 * interface_stdin.cpp
 *
 * libRadtranの標準入力に渡すファイルを作る
 *
 */

#include<iostream>
#include<fstream>

#include"interface.h"

int save_stdin(std::string path, ParamStdin param){
	std::ofstream ofs(path);

	if(!ofs){
		std::cerr << "Failed to create file '" << path << "'" << std::endl;
		return 1;
	}
		
	ofs << "atmosphere_file ../data/atmmod/afglus.dat\n";
	ofs << "source solar    ../data/solar_flux/atlas_plus_modtran\n\n";
	ofs << "rte_solver " << param.solver <<"\n";
	if(param.solver == "mystic"){
		ofs << "mc_spherical 1D\n";
		ofs << "mc_photons " << param.mc_photons << "\n";
		ofs << "mc_backward\n";
		ofs << "#mc_backward_output eup\n#mc_escape off\n\n";
	}
	ofs << "wavelength " << param.wavelength << "\n\n";
	ofs << "zout TOA\n";/* fixed */
	ofs << "sza " << param.sza << "\n";
	ofs << "phi0 " << param.phi0 << "\n\n";
	ofs << "umu " << param.umu << "\n";
	ofs << "phi " << param.phi << "\n";
	if(param.solver != "mystic"){
		ofs << param.additional << "\n";
	}
	

	ofs.close();
	return 0;
}
