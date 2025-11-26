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


#include"wrapper.h"



double wrapper(const std::vector<double> &Coef, std::vector<double> &grad, void* raw_Args){
	WrapperArgs* args = static_cast<WrapperArgs*>(raw_Args);
	/* ==== setting atmosphere ==== */
	args->pAtm = Nair_to_atmosphere(
		args->Nheights,
		args->dt,
		args->tparr,
		args->planet,
		Coef,
		0.0
		);	
		saveParamAtmosphere(args->PATH_ATMOSPHERE, args->pAtm, args->Nheights, args->atmosphere_precision);
	/* ==== */
	double *radiance = new double [args->Nheights];/* シミュレーション結果 */
	for(int i=args->i_bottom; i<=args->i_top; i++){
	/* ==== setting sensor ==== */
		Geocoordinate tp = args->tparr[i];
		std::cout << "Tangential point:\n";
		std::cout << "\tlat:" << tp.latitude() << "\n\tlon:" << tp.longitude() << "\nt\taltitude:" << tp.altitude() << "\n\talpha:" << tp.alpha()*Rad2deg << std::endl;


		AndoLab::Vector3d <double> *crosspts = new AndoLab::Vector3d <double> [2];
		crosspts = Across_point_atmosphere(args->planet, args->satellite, tp.r(), args->TOA_height); 
		Geocoordinate crosspt(args->planet, args->satellite, crosspts[0]);/* TOAと視線の交点を求める */

		std::cout << "crosspoint_of_atmosphere:\n\tlat:" << crosspt.latitude() << "\n\tlon:" << crosspt.longitude() << "\n\taltitude:" << crosspt.altitude() << std::endl;
		
		AndoLab::solar_direction(crosspt.latitude(), crosspt.longitude(), args->dt.DOY(), args->dt.Hour(), &args->pStdin.sza, &args->pStdin.phi0);/* tangential point での太陽方向を求める */
		std::cout << "sza:" << args->pStdin.sza << " phi0:" << args->pStdin.phi0 << std::endl;
		sensor_direction(args->satellite, crosspt.r(), &args->pStdin.umu, &args->pStdin.phi);/* crosspt からみた衛星方向を元に、視線方向の局所鉛直からの角（オフナディア角）を求める */
		double sensor_theta;
		sensor_theta = acos(args->pStdin.umu) * Rad2deg;
		std::cout << "sonsor_direction:\n\tsensor_theta:" << sensor_theta <<"\n\tphi:" << args->pStdin.phi << std::endl;
		std::cout << "cos(sensor_theta) = umu = " << args->pStdin.umu << std::endl;
	
		save_stdin(args->PATH_STDIN, args->pStdin);/* 座標情報を入力ファイルにセーブ */
	/* ==== */
	/* ==== acquiring radiance from libRadtran ==== */
		
		for(int i=0; i<args->Nheights; i++){
			radiance[i] = 0.0;/* initialize */
		}
	
		/* delete_mystic_rad(); */
		execute_uvspec(args->DIR_UVSPEC, args->PATH_STDIN, args->PATH_STDOUT, args->FLAG_UNDISPLAY_LOG, args->DIR_LOG);
		if(args->pStdin.solver == "mystic"){
			radiance[i] = read_mystic_rad(args->DIR_UVSPEC, 105);/* TODO この層番号の決め方がいまいちわからない 0から100km, 1kmごとであればTOAで105 */
		}else{
			radiance[i] = read_stdout(args->PATH_STDOUT, 0);
		}	
	/* ==== */
	std::cout << "Tangential height: " << tp.altitude() << " [m]\nRadiance: " << radiance[i] << "\n----" << std::endl;
	}

/* ==== saving results ==== */
	std::string path_result = save_path(args->DIR_RESULT, args->secid, args->dt, args->obs_index + 1);
	save_result(path_result, args->secid, args->on_ground, args->Nheights, args->heights, radiance);/* TODO 最適化を回し始めたらいらない */
	save_params(args->DIR_RESULT, args->secid, args->PATH_ATMOSPHERE, "_atm.txt");/* atmosphereも保存しておく */
	save_params(args->DIR_RESULT, args->secid, args->PATH_STDIN, "_stdin.txt");
	save_params(args->DIR_RESULT, args->secid, args->PATH_CONFIG, "_config.conf");
/* ==== */
/* ==== fitting result ==== */
	double offset = fit::mean(args->Nheights, args->heights, args->obs.Data(), args->offset_bottom_height, args->TOA_height);
	double* a_offset = fit::obtain_fitting_coefficient(args->obs.Data(), radiance, args->min_index, args->max_index, offset);
	double* fitted = fit::apply_fitting(args-> Nheights, radiance, a_offset);
	double* smoothed = fit::running_mean_log(args->Nheights, 3, fitted);/* 3 は移動平均をとる数 */
	
	double log_square_error = fit::root_mean_square_log_error( args->min_index, args->max_index, args->obs.Data(), smoothed );

	return log_square_error;	
}


