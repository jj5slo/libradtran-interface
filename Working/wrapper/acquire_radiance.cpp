#include "wrapper.h"

double acquire_radiance(
	const std::string DIR_UVSPEC,
	const std::string PATH_STDIN,
	const std::string PATH_STDOUT,
	const int FLAG_UNDISPLAY_LOG,
	const std::string DIR_LOG, 
	const std::string solver
){
	double radiance = 0.0;

	execute_uvspec(DIR_UVSPEC, PATH_STDIN, PATH_STDOUT, FLAG_UNDISPLAY_LOG, DIR_LOG);
		
	if(solver == "mystic"){
		radiance = read_mystic_rad(DIR_UVSPEC, 105);/* TODO この層番号の決め方がいまいちわからない 0から100km, 1kmごとであればTOAで105 */
	}else{
		radiance = read_stdout(PATH_STDOUT, 0);
	}
	return radiance;
}

