/*
 * wrapper.cpp
 *
 *
 */

/*
 *	すべきこと
 *			
 *		fitを組み込む
 * 		
 */
#include <cmath>
#include <regex>

#include "wrapper.h"
#include "filematch.h"


double core(void* raw_Args){
	WrapperArgs* args = static_cast<WrapperArgs*>(raw_Args);
//	std::cout <<std::endl<< args->Nheights;
//	std::cout <<std::endl<< args->atm_Nheights;
//	std::cout <<std::endl<< args->DIR_UVSPEC;
//	std::cout <<std::endl<< args->PATH_STDIN;
//	std::cout <<std::endl<< args->PATH_STDOUT;
//	std::cout <<std::endl<< args->PATH_ATMOSPHERE;
//	std::cout <<std::endl<< args->DIR_RESULT;/* for save */
//	std::cout <<std::endl<< args->PATH_CONFIG;/* for save */
//	std::cout <<std::endl<< args->FLAG_UNDISPLAY_LOG;
//	std::cout <<std::endl<< args->DIR_LOG;
//	std::cout <<std::endl<< args->i_bottom;/* for fit, error */
//	std::cout <<std::endl<< args->i_top;/* for fit, error */
//	std::cout <<std::endl<< args->atm_i_bottom;
//	std::cout <<std::endl<< args->atm_i_top;
//	std::cout <<std::endl<< args->TOA_height;
//	std::cout <<std::endl<< args->offset_bottom_height;/* for fit */
//	std::cout <<std::endl<< args->atmosphere_precision;
//	std::cout <<std::endl<< args->secid;/* for save */
//	std::cout <<std::endl<< args->obs_index;/* for save */
//	std::cout <<std::endl<< args->N_running_mean;/* for fit */
//	std::cout <<std::endl<< args->number_of_iteration;/* NLopt */
//	std::cout << std::endl;
	const int running_mean_extra = args->N_running_mean / 2;/* ( N - 1 ) / 2 */
	const int i_bottom_rad = args->i_bottom - running_mean_extra;/* TODO TODO NOW fitは上は計算済みのものを使う */
	const int i_top_rad    = args->i_top    + running_mean_extra;
	
	double* radiance   = new double [args->Nheights];/* シミュレーション結果 */
	double* rad_NN     = new double [args->Nheights];/* シミュレーション結果 */
	double* rad_NN_var = new double [args->Nheights];/* シミュレーション結果 */
	double** radiance_each_wl = new double*[args->SRWeights.N()];
	double** rad_NN_each_wl = new double*[args->SRWeights.N()];
	double** rad_NN_sd_each_wl = new double*[args->SRWeights.N()];
	int**    photons_each_wl = new int*[args->SRWeights.N()];
	int**    seconds_each_wl = new int*[args->SRWeights.N()];
	for(int ii=0; ii<args->SRWeights.N(); ii++){
		radiance_each_wl[ii] = new double[args->Nheights];
		rad_NN_each_wl[ii] = new double[args->Nheights];
		rad_NN_sd_each_wl[ii] = new double[args->Nheights];
		photons_each_wl[ii] = new int[args->Nheights];
	}
	for(int i=0; i<args->Nheights; i++){
		radiance[i] = 0.0;/* initialize */
		rad_NN[i] = 0.0;/* initialize */
		rad_NN_var[i] = 0.0;/* initialize */
		for(int ii=0; ii<args->SRWeights.N(); ii++){
			radiance_each_wl[ii][i] = 0.0;
			rad_NN_each_wl[ii][i] = 0.0;
			rad_NN_sd_each_wl[ii][i] = 0.0;
			photons_each_wl[ii][i] = 0;
		}
	}
	for(int i=i_bottom_rad; i<=i_top_rad; i++){
	/* ==== setting sensor ==== */
		std::cout << "setting sensor..." << std::endl;
		Geocoordinate tp = args->tparr[i];
		std::cout << "Tangential point:\n";
		std::cout << "\tlat:" << tp.latitude() << "\n\tlon:" << tp.longitude() << "\nt\taltitude:" << tp.altitude() << "\n\talpha:" << tp.alpha()*Rad2deg << std::endl;


		AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
		crosspts = Across_point_atmosphere(args->planet, args->satellite, tp.r(), args->TOA_height); 
		Geocoordinate crosspt(args->planet, args->satellite, crosspts[0]);/* TOAと視線の交点を求める */

		std::cout << "crosspoint_of_atmosphere:\n\tlat:" << crosspt.latitude() << "\n\tlon:" << crosspt.longitude() << "\n\taltitude:" << crosspt.altitude() << std::endl;
		
		AndoLab::solar_direction(crosspt.latitude(), crosspt.longitude(), args->dt.DOY(), args->dt.HourWithDecimal(), &args->pStdin.sza, &args->pStdin.phi0);/* tangential point での太陽方向を求める */
		std::cout << "sza:" << args->pStdin.sza << " phi0:" << args->pStdin.phi0 << std::endl;
		sensor_direction(args->satellite, crosspt.r(), &args->pStdin.umu, &args->pStdin.phi);/* crosspt からみた衛星方向を元に、視線方向の局所鉛直からの角（オフナディア角）を求める */
		double sensor_theta;
		sensor_theta = acos(args->pStdin.umu) * Rad2deg;
		std::cout << "sonsor_direction:\n\tsensor_theta:" << sensor_theta <<"\n\tphi:" << args->pStdin.phi << std::endl;
		std::cout << "cos(sensor_theta) = umu = " << args->pStdin.umu << std::endl;
		if(args->FLAG_adapt_mc_photons == 1){
//			args->pStdin.mc_photons = ;/* TODO */
		}

		for(int j=0; j<args->SRWeights.N(); j++){
			double rad_wavelength      = 0.0;
			double rad_wavelength_NN   = 0.0;
			double rad_wavelength_NN_sd  = 0.0;
			double rad_wavelength_spc = 0.0;
			args->pStdin.wavelength = args->SRWeights.wavelength(j);
			save_stdin(args->PATH_STDIN, args->pStdin);/* 座標情報を入力ファイルにセーブ */
			/* ==== */
			/* ==== acquiring radiance from libRadtran ==== */
			const std::regex TARGET_PATTERN(R"(mc.*\.rad(\.std)?$)");
			deleteMatchingFiles(args->DIR_UVSPEC, TARGET_PATTERN);
			deleteMatchingFiles(args->DIR_UVSPEC, std::regex(R"(mc.*\.spc$)"));
			std::filesystem::remove(args->DIR_LOG+"libRadtran.log");
			auto starttime = std::chrono::system_clock::now();

			std::cout << "acquiring radiance from libRadtran..." << std::endl;	
			execute_uvspec(args->DIR_UVSPEC, args->PATH_STDIN, args->PATH_STDOUT, args->FLAG_UNDISPLAY_LOG, args->DIR_LOG);
			
			auto endtime = std::chrono::system_clock::now();
			int elapsed_sec = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count());
			int photons = getLastPhotonCountFromLog(args->DIR_LOG+"libRadtran.log");


			if(args->pStdin.solver == "mystic" || args->pStdin.solver == "mystic_plainparallel"){
				rad_wavelength_NN     = read_mystic_rad_NN(args->DIR_UVSPEC);
				rad_wavelength_NN_sd  = read_mystic_rad_sd(args->DIR_UVSPEC);
				rad_wavelength_spc    = read_mystic_rad_spc(args->DIR_UVSPEC);
				rad_wavelength = rad_wavelength_spc;
				std::cout << "rad_NN: " << std::setprecision(7) << rad_wavelength_NN << " ratio: " << rad_wavelength_spc/rad_wavelength_NN << std::endl;
				std::cout << "rad_NN_sd: " << std::setprecision(7) << rad_wavelength_NN_sd << " sd/rad_NN:" << rad_wavelength_NN_sd/rad_wavelength_NN << std::endl;
			}else{
				rad_wavelength = read_stdout(args->PATH_STDOUT, 0);
			}	
	/* ==== */
			std::cout << "Tangential height: " << tp.altitude() << " [m]\nWavelength: " << args->pStdin.wavelength << "[nm], Weight: " << args->SRWeights.weight(j) << ", Radiance: " << rad_wavelength << "\n----" << std::endl;
			radiance_each_wl[j][i]  = rad_wavelength;
			rad_NN_each_wl[j][i]    = rad_wavelength_NN;
			rad_NN_sd_each_wl[j][i] = rad_wavelength_NN_sd;
			photons_each_wl[j][i]   = photons;
			seconds_each_wl[j][i]   = elapsed_sec;
			radiance[i]   += args->SRWeights.weight(j) / args->SRWeights.sum_weights() * rad_wavelength;
			rad_NN[i]     += args->SRWeights.weight(j) / args->SRWeights.sum_weights() * rad_wavelength_NN;
			rad_NN_var[i] += args->SRWeights.weight(j) / args->SRWeights.sum_weights() * args->SRWeights.weight(j) / args->SRWeights.sum_weights() * rad_wavelength_NN_sd*rad_wavelength_NN_sd;
		}

		std::cout << "Sum: Tangential height: " << tp.altitude() << " [m]\nRadiance: " << radiance[i] << "\n----" << std::endl;

	}

/* ==== saving parameters ==== */
	std::string identifier = args->secid +"_"+ std::to_string(args->number_of_iteration);
	std::cout << "saving results..." << std::endl;
	std::string path_result = save_path(args->DIR_RESULT, identifier, args->dt, args->obs_index + 1);
	
	save_params(args->DIR_RESULT, args->secid, args->PATH_ATMOSPHERE, "_atm"+std::to_string(args->number_of_iteration)+".dat");/* atmosphereも保存しておく */
	save_params(args->DIR_RESULT, args->secid, args->PATH_STDIN, "_stdin.txt");
	save_params(args->DIR_RESULT, args->secid, args->PATH_CONFIG, "_config.conf");
	args->SRWeights.save(args->DIR_RESULT+"/"+args->secid+"_SRF.dat");

	std::string RawEachWL_header = "# height ";
	for(int ii=0; ii<args->SRWeights.N(); ii++){
		RawEachWL_header += std::to_string(args->SRWeights.wavelength(ii)) + " ";
	}
	RawEachWL_header += "[nm]\n";
	RawEachWL_header += "#        rad rad(NN.rad) sd(NN)\n";
	double** height_radiance_each_wl = new double* [3*(args->SRWeights.N())+1];
	height_radiance_each_wl[0] = args->heights;
	for(int ii=0; ii<args->SRWeights.N(); ii++){
		height_radiance_each_wl[3*ii+1] = radiance_each_wl[ii];
		height_radiance_each_wl[3*ii+2] = rad_NN_each_wl[ii];
		height_radiance_each_wl[3*ii+3] = rad_NN_sd_each_wl[ii];
	}
	readwrite::save_data(args->DIR_RESULT+"/"+identifier+"_RawEachWL.dat", RawEachWL_header, args->Nheights, 3*args->SRWeights.N()+1, height_radiance_each_wl, 8);

	std::string PhotonsEachWL_header = "# height ";
	for(int ii=0; ii<args->SRWeights.N(); ii++){
		PhotonsEachWL_header += std::to_string(args->SRWeights.wavelength(ii)) + " ";
	}
	RawEachWL_header += "[nm]\n";
	int** height_photons_seconds_each_wl = new int* [2*(args->SRWeights.N())+1];
	height_photons_seconds_each_wl[0] = new int[args->Nheights];
	for(int ii=0; ii<args->Nheights; ++ii){
		height_photons_seconds_each_wl[0][ii] = static_cast<int>(std::round(args->heights[ii]));
	}
	for(int ii=0; ii<args->SRWeights.N(); ii++){
		height_photons_seconds_each_wl[2*ii+1] = photons_each_wl[ii];
		height_photons_seconds_each_wl[2*ii+2] = seconds_each_wl[ii];
	}
	readwrite::save_data(args->DIR_RESULT+"/"+identifier+"_PhotonsSecondsEachWL.dat", PhotonsEachWL_header, args->Nheights, args->SRWeights.N()+1, height_photons_seconds_each_wl);
	
	delete[] height_radiance_each_wl;
	delete[] height_photons_seconds_each_wl[0];
	delete[] height_photons_seconds_each_wl;
	for(int ii=0; ii<args->SRWeights.N(); ii++){
		delete[] radiance_each_wl[ii];
		delete[] rad_NN_each_wl[ii];
		delete[] rad_NN_sd_each_wl[ii];
		delete[] photons_each_wl[ii];
		delete[] seconds_each_wl[ii];
	}
	delete[] radiance_each_wl;
	delete[] rad_NN_each_wl;
	delete[] rad_NN_sd_each_wl;
	delete[] photons_each_wl;
	delete[] seconds_each_wl;
	
/* ==== */
/* ==== fitting results ==== */
	std::cout << "fitting results..." << std::endl;
	double* smoothed = fit::running_mean_log(args->Nheights, args->N_running_mean, radiance);
/* -- merging radiance (including upper stage) -- */
	/* running_mean_extra(通常1)だけ範囲外も計算しているので、その端点i_top_rad - 1が一致するように過去の結果(上の層束)を合わせ直す(running_mean_extraが1より大きいのときは平均とかにしたほうがい) */
//	double fit_radiance_of_upper_stage_coefficient = 1.0;
//	if(1.0e-12 <= std::abs(args->upper_radiance_smoothed[i_top_rad-1])){/* TODO */
//		fit_radiance_of_upper_stage_coefficient = smoothed[i_top_rad-1]  / args->upper_radiance_smoothed[i_top_rad-1];
//		std::cout << "coefficient to connect upper radiance: " << fit_radiance_of_upper_stage_coefficient << std::endl;
//	}
//	for(int i=i_top_rad; i<args->Nheights; i++){
//		smoothed[i] = args->upper_radiance_smoothed[i] * fit_radiance_of_upper_stage_coefficient;
//	}
//	for(int i=i_bottom_rad; i<args->Nheights; i++){
//		args->radiance_smoothed[i] = smoothed[i];
//	}
/* -- */
//	double offset = fit::mean(args->Nheights, args->heights, args->obs.Data(), args->offset_bottom_height, args->offset_top_height);
	double offset = 0.0;
	double* a_offset = fit::obtain_fitting_coefficient(args->obs.Data(), smoothed, args->fit_i_bottom, args->fit_i_top, offset);
	double* fitted = fit::apply_fitting(args->Nheights, smoothed, a_offset);

	double* fitted_var = fit::running_mean(args->Nheights, args->N_running_mean, rad_NN_var);
	for(int ii=0; ii<args->Nheights; ++ii){
		fitted_var[ii] = fitted_var[ii] / (double)args->N_running_mean;/* 平均の分散なのでさらに割る */
		fitted_var[ii] = fitted_var[ii] * radiance[ii] / rad_NN[ii] * radiance[ii] / rad_NN[ii];/* radiance / rad_NN を掛ける（分散なので2乗） */
		fitted_var[ii] = fitted_var[ii] * a_offset[0] * a_offset[0];/* fitted/smoothed を掛ける（分散なので2乗） */
	}
	
	double** processed_results = new double* [8];
	double log_square_error = fit::root_mean_square_log_error( args->i_bottom, args->i_top, args->obs.Data(), fitted );
	
	processed_results[0] = args->heights;
	processed_results[1] = args->obs.Data();
	processed_results[2] = radiance;
	processed_results[3] = smoothed;
	processed_results[4] = fitted;
	processed_results[5] = rad_NN;
	processed_results[6] = rad_NN_var;
	processed_results[7] = fitted_var;
	double ld_alpha = args->on_ground.alpha();
	std::string header = 
		"# secid: " + args->secid + "\n"
		+"# number_of_iteration: " + std::to_string(args->number_of_iteration) + "\n"
		+ "# longitude: " + std::to_string(args->on_ground.longitude()) + "\n"
		+ "# latitude: " + std::to_string(args->on_ground.latitude()) + "\n"
		+ "# ld_alpha: " + std::to_string(ld_alpha) + "[rad] = " + std::to_string(ld_alpha*Rad2Deg) + "deg" + "\n"
		+ "# date: " + std::to_string(args->dt.Year())+" "+std::to_string(args->dt.Month())+" "+std::to_string(args->dt.Date()) +"\n"
		+ "# time: " + std::to_string(args->dt.Hour())+":"+std::to_string(args->dt.Minute())+":"+std::to_string(args->dt.Second())+" UT, Hour="+std::to_string(args->dt.HourWithDecimal()) +"\n"
		+ "# sza_on_ground: " + std::to_string(args->sza_on_ground) +"\n"
		+ "# phi0_on_ground: " + std::to_string(args->phi0_on_ground) + "\n"
		+ "# i_bottom: " + std::to_string(args->i_bottom) + ", i_top: " + std::to_string(args->i_top) + "\n"
		+ "# atm_i_bottom: " + std::to_string(args->atm_i_bottom) + ", atm_i_top: " + std::to_string(args->atm_i_top) + "\n"
		+ "# a: " + std::to_string(a_offset[0]) + ", offset: " + std::to_string(a_offset[1]) + "\n"
		+ "# N_running_mean: " + std::to_string(args->N_running_mean) + "\n"
		+ "# log_square_error: " + std::to_string(log_square_error) + "\n"
		+ "# height observed sumulated smoothed fitted sim_raw sim_raw_variance fitted_variance\n";
	readwrite::save_data(path_result, header, args->Nheights,  8, processed_results, 8);/* 最適化を回し始めたら不要、/tmp/に入れてもいいかも */
	delete[] processed_results;
	
	
	delete[] radiance;
	delete[] rad_NN;
	delete[] rad_NN_var;
	delete[] fitted;
	delete[] fitted_var;
	return log_square_error;	
}


