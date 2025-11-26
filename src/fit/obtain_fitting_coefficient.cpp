
#include"fit.h"


// int main(int argc, char *argv[]){
// 	
// 	if(argc != 2){
// 		std::cerr << "Usage ./fit paths.txt" << std::endl;
// 	}
// 	std::string path = argv[1];
// 
// 	std::string paths = argv[1];
// 	std::ifstream ifs(paths);
// 	std::string line;
// 	
// 	while(std::getline(ifs, line)){
// 		if(line[0] != '#'){
// 			std::istringstream iss_read(line);
// 			std::string token;
// 			while(iss_read >> token){
// 				fitting_result(token);
// 			}
// 		}
// 	}
// 
// 	return 0;
// }

double* fit::obtain_fitting_coefficient(double* obs, double* sim, int min_index, int max_index, double offset){
	
	double* a_offset = new double[2];
	a_offset[0] = 0.0;
	a_offset[1] = offset;

	nlopt::opt opt( nlopt::LN_NELDERMEAD, 1 );
	
	fit::SLE_Param slep;
	slep.number_of_iteration = 0;
	slep.x = obs;
	slep.y = sim;
	slep.offset = offset;
	slep.min_index = min_index;
	slep.max_index = max_index;

	opt.set_min_objective( fit::least_square_log_error, (void*)(&slep) ); 
	opt.set_xtol_rel(1.0e-6);/* TODO */
	std::vector<double> x(1, 5.0e3);/* 初期値 */
	double minf;

	try {
		slep.number_of_iteration = 0;
		nlopt::result result = opt.optimize(x, minf);
	} catch (std::exception &e){
		std::cout << "NLopt for obtaining fitting coefficient failed. : " << e.what() << std::endl;
	}
	a_offset[0] = x[0];
	std::cout << "fitting_result:\n\ta: " << a_offset[0] << "\n\toffset: " << a_offset[1] << "\n\tminf: " << minf << std::endl;
	return a_offset;
}

