/*
 * save.cpp
 */

#include"save.h"


int save_result(std::string path, int Nheights, double* heights, Observed obsd, double* radiance){
	

	std::pair<double*, double*> rad_minmax = std::minmax_element(radiance, radiance+Nheights);
	double* rad_min = rad_minmax.first;
	double* rad_max = rad_minmax.second;
	int rad_min_i = rad_min - radiance;
	int rad_max_i = rad_max - radiance;
	double obs_rminv = obsd.Data(heights[rad_min_i]);
	double obs_rmaxv = obsd.Data(heights[rad_max_i]);

	double scaling_factor = (obs_rmaxv - obs_rminv) / (*rad_max - *rad_min);

	std::cout << "rad_min rad_max rad_min_i rad_max_i obs_rminv obs_rmaxv scaling_factor\n" << rad_min <<" "<< rad_max <<" "<< rad_min_i <<" "<< rad_max_i <<" "<< obs_rminv <<" "<< obs_rmaxv <<" "<< scaling_factor << std::endl;

	double* radiance_fitted = new double[Nheights];
	for(int i=0; i<Nheights; i++){
		radiance_fitted[i] = radiance[i] * scaling_factor + (obs_rminv - *rad_min) ;
	}
		
	std::cout << "height observed simulated simulated-fitted" << std::endl;

	std::ofstream ofs(path);


	for(int i=0; i<Nheights; i++){
		ofs << heights[i] << " " << obsds[obs_index].Data(heights[i]) << " " << radiance[i] << " " << radiance_fitted[i] << "\n";
	}



}
