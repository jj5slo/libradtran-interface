#include "fit.h"



double* fit::obtain_lowerbound_fitting_coefficient(double* obs, double* sim, int min_index, int max_index, double offset){
	std::cout << "obtain_lowerbound_fitting_coefficient: fitting in [" << min_index << ", " << max_index << "]" << std::endl;

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
		std::cerr << "NLopt for obtaining fitting coefficient failed. : " << e.what() << std::endl;
	}
	a_offset[0] = x[0];
	std::cout << "fitting_result:\n\ta: " << a_offset[0] << "\n\toffset: " << a_offset[1] << "\n\tminf: " << minf << std::endl;
	return a_offset;
}


/* 不等式制約関数 */
/*
 *  m: 制約の数
 *  n: 最適化パラメタ数
 *
 */
void constraint_func(unsigned m, double* result, unsigned n, double* grad, void* data){
	fit::SLE_Param* slep = static_cast<fit::SLE_Param*>(data);
	
	for(unsigned i = 0; i < m; ++i){
		result[i] = slep->y - slep->x;
	}
}
