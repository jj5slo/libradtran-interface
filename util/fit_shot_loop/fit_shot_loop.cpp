/*
 * shot_loopで出した輝度をフィッティングする
 *               RMSE 
 * 出力: SZA PHI 0K +5K -5K +10K -10K
 *
 */


#include <iostream>
#include <fstream>

#include "readwrite.h"
#include "fit.h"

int main(int argc, char *argv[]){
	if(argc != 10){
		std::cerr << "Usage: ./fit_shot_loop reference.dat SZA PHI FILE-10 FILE-5 FILE0 FILE+5 FILE+10 OUTPUT" << std::endl;
		return 1;
	}
	const std::string PATH_REF = std::string(argv[1]);
	const double SZA = std::stod(argv[2]);	
	const double PHI = std::stod(argv[3]);	
	int Nfiles = 5;
	const std::string paths[5] =
	{
		std::string(argv[4]),
		std::string(argv[5]),
		std::string(argv[6]),
		std::string(argv[7]),
		std::string(argv[8])
	};
	const std::string PATH_OUTPUT = std::string(argv[9]);
	
	int Nlines = 0;
	int Ncolumns = 0;
	std::string header = "";
	double** refdata = readwrite::read_data(PATH_REF, header, Nlines, Ncolumns);
	double* rmse = new double[7];
	rmse[0] = SZA;
	rmse[1] = PHI;

	for(int i=0; i<Nfiles; ++i){
		double** anotherdata = readwrite::read_fixed_data(paths[i], header, Nlines, Ncolumns);
		
		// std::cout << "obtain fitting_coefficient: fitting in [" << refdata[0][3] << ", " << refdata[0][7] << "]" << std::endl;

		nlopt::opt opt( nlopt::LN_NELDERMEAD, 1 );
		
		fit::SLE_Param slep;
		slep.number_of_iteration = 0;
		slep.x = refdata[1];
		slep.y = anotherdata[1];
		slep.offset = 0.0;
		slep.min_index = 3;
		slep.max_index = 7;

		opt.set_min_objective( fit::least_square_log_error, (void*)(&slep) ); 
		opt.set_xtol_rel(1.0e-6);/* TODO */
		std::vector<double> x(1, 3.0);/* 初期値 */
		double minf;

		try {
			slep.number_of_iteration = 0;
			nlopt::result result = opt.optimize(x, minf);
		} catch (std::exception &e){
			std::cerr << "NLopt for obtaining fitting coefficient failed. : " << e.what() << std::endl;
		}
		rmse[2+i] = std::sqrt(minf);
	}
	
	std::ofstream ofs(PATH_OUTPUT, std::ios::app);
	if(ofs.is_open()){
		ofs << std::endl;
		for(int i=0; i<7; ++i){
			ofs << rmse[i] << " ";
		}
		ofs.close();
	}
	
	delete[] refdata;
	delete[] rmse;

}
