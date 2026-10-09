#include "fit.h"


int main(int argc, char* argv[]){
	if(argc != 3){
		std::cerr << "Usage ./fit paths.txt column" << std::endl;
		return 1;
	}

	std::string paths = argv[1];
	int COLUMN = std::stod(argv[2]);

	std::string path;
//	std::string* paths_save = new std::string[Nfiles];/* 平均化して保存したファイルの一覧 */

	std::ifstream ifs(paths);
	std::string line;

//	double*** data_arr = new double** [Nfiles];
	int i_file = 0;
	int Nlines = 0;
	int Ncolumns = 0;
	while(std::getline(ifs, line)){
		if(line[0] != '#'){
			std::istringstream iss_read(line);
			std::string token;

			while(iss_read >> token){
				path = token;

				/* -- */

//				int Ndata;
				std::string header;
				Nlines = 0;
				Ncolumns = 0;
				double** data = fit::read_result(path, header, Nlines, Ncolumns);
//				data_arr[i_file] = data;
//				double** running_means = new double* [4];/* 0=3, 1=5, 2=3(log), 3=5(log) */
//				running_means[0] = fit::running_mean(Nlines, 3, data[COLUMN]);
//				running_means[1] = fit::running_mean(Nlines, 5, data[COLUMN]);
//				double* data_col_log = new double [Nlines];/* データ3列目（フィッティング後）の対数 */
//				for(int i=0; i<Nlines; i++){
//					data_col_log[i] = std::log10(data[COLUMN][i]);
//				}
//				running_means[2] = fit::running_mean(Nlines, 3, data_col_log);
//				running_means[3] = fit::running_mean(Nlines, 5, data_col_log);
//				for(int i=0; i<Nlines; i++){
//					running_means[2][i] = std::pow(10.0, running_means[2][i]);
//					running_means[3][i] = std::pow(10.0, running_means[3][i]);
//				}
				double* data_col_log = new double[Nlines];/* データcol列目（フィッティング後）の対数 */
				for(int i=0; i<Nlines; i++){
					data_col_log[i] = std::log10(data[COLUMN][i]);
				}
				double* running_mean = fit::running_mean(Nlines, 3, data_col_log);
				for(int i=0; i<Nlines; i++){
					running_mean[i] = std::pow(10.0, running_mean[i]);
				}
				
				/* ==== ファイルに保存 ==== */	
//				header = "# RUNNING_MEAN\n"+header + "#height observed simulated sim(fitted) sim(running_mean_3) sim(running_mean_5) sim(running_mean_3_log) sim(running_mean_5_log)\n";
//				paths_saved[i_file] = path+"_runningmean.dat";
//				fit::save_data_and_result(paths_saved[i_file], header, Nlines, Ncolumns, data, 4, running_means);
				header = "# RUNNING_MEAN\n"+header + "#height observed simulated sim(fitted) sim(running_mean_3_log)\n";
				std::string path_save = path+"_runningmean.dat";
				fit::save_data_and_result( path_save, header, Nlines, Ncolumns, data, running_mean);
				/* ==== */
				/* ^^ */
			}
		}
		i_file++;
	}
	ifs.close();
	
//	double** averaged = new double* [Ncolumns];
//	for(int i=0; i<Ncolumns; i++){
//		averaged[i] = new double [Nlines];
//	}
//	double* tmp = new double[Nfiles];
//	for(int k=0; k<Nlines; k++){
//		for(int i=0; i<Nfiles; i++){
//			tmp[i] = data_arr[i][3][k];
//		}
//		averaged[3][k] = fit::mean(Nfiles, tmp);
//		for(int i=0; i<Nfiles; i++){
//			tmp[i] = data_arr[i][2][k];
//		}
//		averaged[2][k] = fit::mean(Nfiles, tmp);
//	}
//
//	for(int i=0; i<Nlines; i++){
//		std::cout << averaged[2][i] <<" "<< averaged[3][i] << std::endl;
//	}
//	double** save_avr = new double* [3];
//	save_avr[0] = data_arr[0][0];
//	save_avr[1] = averaged[2];
//	save_avr[2] = averaged[3];
//	fit::save_data(paths_saved[0]+"_averaged_trials.dat", "#TangentialHeight sim(averaged) sim(fitted-averaged)\n", Nlines, 3, save_avr);


	return 0;
	



	return 0;
}
