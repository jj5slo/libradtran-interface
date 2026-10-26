#include <iostream>
#include <cstdlib>
#include <algorithm>/* 最大最小用 */
#include <chrono>
#include <filesystem>
#include <memory>
#include <vector>
#include <regex>

#include <bayesopt/bayesopt.hpp>

#include "solar_direction.h"

#include "coordinate.h"
#include "interface.h"
#include "execute.h"
#include "save.h"
#include "get_msis.h"
#include "read_config.h"
#include "wrapper.h"
#include "golden_section_search.h"
#include "avoid_dupe.h"
#include "atmNair_to_temperature.h"
#include "filematch.h"


int main(int argc, char *argv[]){
	if(argc != 5){
		std::cerr << "Usage: ./shot_loop SZA PHI PATH_ATM DIR_RESULT" << std::endl;
		return 1;
	}
	const double SZA = std::stod(argv[1]);
	const double PHI = std::stod(argv[2]);
	const std::string PATH_ATM = std::filesystem::canonical(std::string(argv[3]));
	const std::string DIR_RESULT_RAW = std::string(argv[4]);
	
	std::cout << "SZA: " << SZA << std::endl;
	std::cout << "PHI: " << PHI << std::endl;
	std::cout << "DIR_RESULT: " << DIR_RESULT_RAW << std::endl;
	std::cout << "PATH_ATM: " << PATH_ATM << std::endl;

	/* ==== 保存先作成 ==== */
	const auto starttime = std::chrono::system_clock::now();
	const auto startsec = std::chrono::duration_cast<std::chrono::seconds>(starttime.time_since_epoch()).count();
	std::string secid = std::to_string(startsec);
	
	int ifdupe = avoid_dupe(DIR_RESULT_RAW + "/" + secid + "/");
	if(ifdupe){
		secid = secid + std::to_string(ifdupe);
	}
	const std::string DIR_RESULT = DIR_RESULT_RAW + "/" + secid + "/";
	std::cerr << "create_directory " << DIR_RESULT << std::endl;
	std::filesystem::create_directory(DIR_RESULT);
	/* ==== */

	/* ==== 開始時刻保存 ==== */
	std::ofstream timestamp_file(DIR_RESULT+"/timestamp.txt");
	if(!timestamp_file){
		std::cerr << "timestamp.txt could not make!" << std::endl;
		return 1;
	}
	timestamp_file << "start: " << startsec << std::endl;
	timestamp_file.close();
	/* ==== */
	/* ==== 地球、衛星の設定 ==== */
	const auto earth = PlanetParam( 6370.e3 );
	const auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
	/* ==== */

	/* ==== 見る向きの処理 ==== */
	const int i_TOP    = 50+3;
	const int i_BOTTOM = 45-3;
	const int Nheights = i_TOP - i_BOTTOM;
	double* heights = new double[Nheights];
	for(int i=0; i<Nheights; ++i){
		heights[i] = 1.0*(double)(i+i_BOTTOM);
	}
	double ld_alpha = 0.0;/* 北極 */
	Geocoordinate *tparr = new Geocoordinate[Nheights];
	LookingDirection ld;
	for(int i=0; i<Nheights; i++){
		ld.set( ld_alpha, heights[i] );/* 見る場所決め */
		tparr[i] = ld.tangential_point( earth, himawari);/* tangential point の配列 */
	}
	/* ==== */
	/* ==== 大気設定 ==== */
	std::cerr << "ATMOSPHERE " << PATH_ATM << " IS USED." << std::endl;
	/* ==== */
	
	const std::string PATH_STDIN = "/tmp/TEMPORARY/shot_loop/stdin";
	const std::string PATH_STDOUT = "/tmp/TEMPORARY/shot_loop/stdout";
	const std::string DIR_LOG    = "/tmp/TEMPORARY/shot_loop/";
	const std::string DIR_UVSPEC = "/lhome/sano2/SANO/research/LIBRARIES/libradtran/libr0/libRadtran-2.0.6/bin/";

	/* ==== 輝度計算の設定 ==== */
	const double TOA_height = 100.0e3;
	const int N_running_mean = 3;
//	const int running_mean_extra = N_running_mean / 2;/* ( N - 1 ) / 2 */
	double* radiance    = new double [Nheights];/* シミュレーション結果 */
	double* radiance_sd = new double [Nheights];/* シミュレーション結果 */
	double* rad_NN      = new double [Nheights];/* シミュレーション結果 */
	double* rad_NN_sd   = new double [Nheights];/* シミュレーション結果 */
	int*    photons     = new int[Nheights];
	int*    seconds     = new int[Nheights];
	for(int i=0; i<Nheights; i++){
		radiance[i]    = 0.0;/* initialize */
		radiance_sd[i] = 0.0;/* initialize */
		rad_NN[i]      = 0.0;/* initialize */
		rad_NN_sd[i]  = 0.0;/* initialize */
		photons[i]     = 0;
		seconds[i]     = 0;
	}
	for(int i=0; i<Nheights; i++){
		/* ==== setting sensor ==== */
		std::cout << "setting sensor..." << std::endl;
		Geocoordinate tp = tparr[i];
		std::cout << "Tangential point:\n";
		std::cout << "\tlat:" << tp.latitude() << "\n\tlon:" << tp.longitude() << "\nt\taltitude:" << tp.altitude() << "\n\talpha:" << tp.alpha()*Rad2deg << std::endl;

		AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
		crosspts = Across_point_atmosphere(earth, himawari, tp.r(), TOA_height/1.0e3); 
		Geocoordinate crosspt(earth, himawari, crosspts[0]);/* TOAと視線の交点を求める */
		std::cout << "crosspoint_of_atmosphere:\n\tlat:" << crosspt.latitude() << "\n\tlon:" << crosspt.longitude() << "\n\taltitude:" << crosspt.altitude() << "\n\t theta from z: " << crosspt.r().theta() << std::endl;
		
		double umu = 0.0;
		double phi = 0.0;
		sensor_direction(himawari, crosspt.r(), &umu, &phi);/* crosspt からみた衛星方向を元に、視線方向の局所鉛直からの角（オフナディア角）を求める */
		double sensor_theta = acos(umu) * Rad2deg;
		std::cout << "sonsor_direction:\n\tsensor_theta:" << sensor_theta <<"\n\tphi:" << phi << std::endl;
		std::cout << "cos(sensor_theta) = umu = " << umu << std::endl;
		if( std::abs(phi) > 0.1 ){
			std::cerr << "phi is too large! stopped." << std::endl;
			return 1;
		}
		
		/* ==== stdin を生成 ==== */
		ParamStdin pStdin;
		pStdin.atmosphere_file = PATH_ATM;
		pStdin.wavelength = 470.0;
		pStdin.sza = SZA + crosspt.r().theta(); /* solar */
		pStdin.phi0 = PHI;/* solar */
		pStdin.umu = umu;
		pStdin.phi = phi;/* sensor */
		pStdin.mc_photons   = 100000000;
		pStdin.mc_basename = "mc_shot_loop_";
		pStdin.additional  = "aerosol_default\nmc_vroom on\nmc_std 0.5e-2\nverbose\nmol_abs_param crs\n";

		save_stdin(PATH_STDIN, pStdin);/* 座標情報を入力ファイルにセーブ */
		/* ==== */
		/* ==== acquiring radiance from libRadtran ==== */
		deleteMatchingFiles(DIR_UVSPEC, std::regex(pStdin.mc_basename + R"(.*\.rad(\.std)?$)"));
		deleteMatchingFiles(DIR_UVSPEC, std::regex(pStdin.mc_basename + R"(.*\.spc$)"));
		std::filesystem::remove(DIR_LOG+"libRadtran.log");
		
		auto starttime = std::chrono::system_clock::now();
		std::cout << "acquiring radiance from libRadtran..." << std::endl;	
		execute_uvspec(DIR_UVSPEC, PATH_STDIN, PATH_STDOUT, 1, DIR_LOG);
		auto endtime = std::chrono::system_clock::now();
		
		seconds[i] = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(endtime - starttime).count());
		photons[i] = getLastPhotonCountFromLog(DIR_LOG+"libRadtran.log");

		radiance[i]     =  read_mystic_rad_spc(DIR_UVSPEC, pStdin.mc_basename, pStdin.wavelength);
		radiance_sd[i]  =  read_mystic_rad_spc_sd(DIR_UVSPEC, pStdin.mc_basename, pStdin.wavelength);
		rad_NN[i]       =  read_mystic_rad_NN(DIR_UVSPEC, pStdin.mc_basename);
		rad_NN_sd[i]    =  read_mystic_rad_NN_sd(DIR_UVSPEC, pStdin.mc_basename);
		std::cout << "rad_NN: " << std::setprecision(7) << rad_NN[i] << " ratio: " << radiance[i]/rad_NN[i] << std::endl;
		std::cout << "rad_NN_sd: " << std::setprecision(7) << rad_NN_sd[i] << " sd/rad_NN: " << rad_NN_sd[i]/rad_NN[i] << " sd/rad_spc: "<< radiance_sd[i]/radiance[i] << std::endl;
		std::cout << "photons: " << photons[i]  << std::endl;
		std::cout << "Tangential height: " << tp.altitude() << " [m]\nWavelength: " << pStdin.wavelength << "[nm]" << ", Radiance: " << radiance[i] << "\n----" << std::endl;
		/* ==== */
	}

	/* ==== saving parameters ==== */
	std::cout << "saving results..." << std::endl;
	const std::string PATH_RESULT = DIR_RESULT+"result_"+secid+"_"+std::to_string(SZA)+"_"+std::to_string(PHI)+".dat";
	
	save_params(DIR_RESULT, secid, PATH_ATM, "_atm.dat");/* atmosphereも保存しておく */
	save_params(DIR_RESULT, secid, PATH_STDIN, "_stdin.txt");

	std::string Photons_header = "# height photons seconds";
	int** height_photons_seconds = new int* [2+1];
	height_photons_seconds[0] = new int[Nheights];
	for(int ii=0; ii<Nheights; ++ii){
		height_photons_seconds[0][ii] = static_cast<int>(std::round(heights[ii]));
	}
	height_photons_seconds[1] = photons;
	height_photons_seconds[2] = seconds;
	readwrite::save_data(DIR_RESULT+"/"+secid+"_PhotonsSeconds.dat", Photons_header, Nheights, 2+1, height_photons_seconds);
	
	for(int i=0; i<3; ++i){
		delete[] height_photons_seconds[i];
	}
	delete[] height_photons_seconds;
	
/* ==== */
/* ==== fitting results ==== */
	double* smoothed = fit::running_mean_log(Nheights, N_running_mean, radiance);
	
	double** processed_results = new double* [6];
	processed_results[0] = heights;
	processed_results[1] = smoothed;
	processed_results[2] = radiance;
	processed_results[3] = radiance_sd;
	processed_results[4] = rad_NN;
	processed_results[5] = rad_NN_sd;
	std::string header = 
		"# secid: " + secid + "\n"
		+ "# ld_alpha: " + std::to_string(ld_alpha) + "[rad] = " + std::to_string(ld_alpha*Rad2Deg) + "deg" + "\n"
		+ "# sza_on_ground: " + std::to_string(SZA) +"\n"
		+ "# phi0_on_ground: " + std::to_string(PHI) + "\n"
		+ "# i_bottom: " + std::to_string(i_BOTTOM) + ", i_top: " + std::to_string(i_TOP) + "\n"
		+ "# N_running_mean: " + std::to_string(N_running_mean) + "\n"
		+ "# height smoothed radiance radiance_sd rad_NN rad_NN_sd\n";
	readwrite::save_data(PATH_RESULT, header, Nheights,  6, processed_results, 8);
	

	for(int i=0; i<6; ++i){
		delete[] processed_results[i];
	}
	delete[] processed_results;
	return 0;
}
