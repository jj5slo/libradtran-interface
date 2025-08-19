/*
 * save.cpp
 */

#include"save.h"

std::string save_path(std::string data_dir, obsDateTime dt){
	std::ostringstream filename;
	filename << "result_" << std::setw(4) << std::setfill('0') << dt.Year() << std::setw(2) << std::setfill('0') << dt.Month() <<  std::setw(2) << std::setfill('0') << dt.Date() << "_" <<   std::setw(2) << std::setfill('0') << dt.Hour() <<  std::setw(2) << std::setfill('0') << dt.Minute() <<  std::setw(2) << std::setfill('0') << dt.Second() << ".txt";
	std::string path = data_dir + filename.str();
	return path;
}

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
		


	std::ofstream ofs(path);

	if(!ofs){
		std::cerr << "Failed to open file '" << path << "'" << std::endl;
		return 1;
	}
	ofs << "#rad_min rad_max rad_min_i rad_max_i obs_rminv obs_rmaxv scaling_factor\n#" << rad_min <<" "<< rad_max <<" "<< rad_min_i <<" "<< rad_max_i <<" "<< obs_rminv <<" "<< obs_rmaxv <<" "<< scaling_factor << "\n";
	ofs << "#height observed simulated simulated-fitted\n";

	for(int i=0; i<Nheights; i++){
		ofs << heights[i] << " " << obsd.Data(heights[i]) << " " << radiance[i] << " " << radiance_fitted[i] << "\n";
	}

	return 0;

}
