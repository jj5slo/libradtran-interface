#include "refit.h"
/*
 *
 *
 *
 *
 */



int main(int argc, char* argv[]){
	std::string PATH;
	int         i_BOTTOM;
	int         i_TOP;
	std::string NEWPATH;
	int         offset_BOTTOM;
	if(argc == 6){
		PATH = std::string(argv[1]);
		i_BOTTOM = atoi(argv[2]);
		i_TOP    = atoi(argv[3]);
		NEWPATH = std::string(argv[4]);
		offset_BOTTOM = atoi(argv[5]);
	}
	else{
		std::cerr << "Usage: ./refit PATH i_BOTTOM i_TOP NEWPATH offset_BOTTOM" << std::endl;
		return 0;
	}

	std::string header = "";
	int Nheights{0};
	int Ncolumns{0};
	double** result = readwrite::read_data(PATH, header, Nheights, Ncolumns);
	if(Ncolumns != 5){
		std::cerr << "Format error ( " << 5 <<" columns required, " << Ncolumns <<" columns detected )" << std::endl;
		for(int i=0; i<Ncolumns; i++){
			delete[] result[i];
		}
		delete[] result;
		return 1;
	}

/* ==== fitting results ==== */
	std::cout << "fitting results..." << std::endl;
	double* heights  = result[0];
	double* observed = result[1];
	double* raw_rad  = result[2];
	double* smoothed = result[3];
	double* fitted   = result[4];
/* -- */
	double offset = fit::mean(Nheights, heights, observed, offset_BOTTOM, Nheights - 1);
	double* a_offset = fit::obtain_fitting_coefficient(observed, smoothed, i_BOTTOM, i_TOP, offset);
	fitted = fit::apply_fitting(Nheights, smoothed, a_offset);
	double** processed_results = new double* [5];
	double log_square_error = fit::root_mean_square_log_error( i_BOTTOM, i_TOP, observed, fitted );
	processed_results[0] = heights;
	processed_results[1] = observed;
	processed_results[2] = raw_rad;
	processed_results[3] = smoothed;
	processed_results[4] = fitted;
	header = header 
		+ "# log_square_error ( index " + std::to_string(i_BOTTOM) + " to " + std::to_string(i_TOP) + "): " + std::to_string(log_square_error) + "\n"
		+ "# height obs raw smoothed fitted\n";
	fit::save_data(NEWPATH, header, Nheights, 5, processed_results);/* 最適化を回し始めたら不要、/tmp/に入れてもいいかも */
	delete[] processed_results;
		
	AndoLab::deallocate_memory2d(result);

	return 0;
}
