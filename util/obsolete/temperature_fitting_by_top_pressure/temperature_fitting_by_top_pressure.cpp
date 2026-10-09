#include <iostream>
#include <fstream>
#include <cmath>
#include <nlopt.hpp>

#include "readwrite.h"
#include "linear_interpolation.h"

#include "interface.h"
#include "atmNair_to_temperature.h"

#include "temperature_fitting_by_top_pressure.h"


int main(int argc, char* argv[]){
	std::string REF_PATH;
	std::string ATM_PATH;
	int         MIN_INDEX;
	int         iTOP;
	if(argc == 5){
		REF_PATH  =           argv[1];
		ATM_PATH  =           argv[2];
		MIN_INDEX = std::stoi(argv[3]);
		iTOP      = std::stoi(argv[4]);
	}
	else{
		std::cerr << "Usage: ./t REFERENCE.dat ATMOSPHERE.dat MIN_INDEX(0startindex) iTOP(0startindex)" << std::endl;
		return 1;
	}
	
	int Nheights = 0;
	ParamAtmosphere* atm = readParamAtmosphere(ATM_PATH, Nheights);

	int Nheights_ref = 0;
	int Ncolumns_ref = 2;
	std::string header = "";
	double** ref_data = read_data(REF_PATH, header, Nheights_ref, Ncolumns_ref);

	PackedParams args;
	args.rawAtm    = atm;
	args.Nheights  = Nheights;
	args.iTOP      = iTOP;
	args.MIN_INDEX = MIN_INDEX;
	args.MAX_INDEX = iTOP - 1;/* TODO */
	args.ref_data  = ref_data;
	args.N_ref     = Nheights_ref;

	double* ref_temps = new double[Nheights];
	for(int i=0; i<args.Nheights; i++){
		ref_temps[i] = linear_interpolation(args.ref_data, args.N_ref, args.rawAtm[i].z);
	//	std::cout << args.rawAtm[i].z << " "<< ref_temps[i] << std::endl;
	}
	
	nlopt::opt opt( nlopt::LN_NELDERMEAD, 1 );
	opt.set_min_objective( objective_func, (void*)(&args) ); 
	opt.set_xtol_rel(1.0e-6);
	opt.set_upper_bounds(1.5);
	opt.set_lower_bounds(0.5);
	/* やりすぎないように現実的な範囲を定める。例えば地上気圧200hPaはやりすぎ。 */

	std::vector<double> coef(1,1.0);
	double minf;
	try{
		nlopt::result result = opt.optimize(coef, minf);
	} catch (std::exception &e){
				std::cout << "NLopt failed : " << e.what() << std::endl;
	}

	std::string save_path = ATM_PATH + "_temp_fit.dat";

	saveParamAtmosphere(save_path, args.resolvedAtm, Nheights, 7);
	
	return 0;
}

double objective_func(const std::vector<double> &Coef, std::vector<double> &grad, void* raw_args){/* for NLopt */
//	double* args = static_cast<WrapperArgs*>(raw_args);
	PackedParams& args = *(static_cast<PackedParams*>(raw_args));

	args.resolvedAtm = call_temperatures(args.rawAtm, args.Nheights, args.iTOP, Coef[0]);
	
	double* temps     = new double[args.Nheights];
	double* ref_temps = new double[args.Nheights];
	for(int i=0; i<args.Nheights; i++){
		temps[i]     = args.resolvedAtm[i].T;
		ref_temps[i] = linear_interpolation(args.ref_data, args.N_ref, args.resolvedAtm[i].z);
//		std::cout <<":"<< temps[i] << " " << ref_temps[i] << std::endl;
	}

	double se = 0.0;
	for(int i=args.MIN_INDEX; i<=args.MAX_INDEX; i++){
		se += (temps[i] - ref_temps[i]) * (temps[i] - ref_temps[i]);
	}
	double mse = se / (double)(args.MAX_INDEX - args.MIN_INDEX + 1);
	double rmse = std::sqrt(mse);
	std::cout << Coef[0] << " " << rmse << std::endl;

	delete[] temps;
	delete[] ref_temps;

	return rmse;
}





	
