#include "../fit.h"


int main(int argc, char* argv[]){
	if(argc != 3){
		std::cerr << "Usage ./fit paths.txt" << std::endl;
	}

	std::string path;

	std::string paths = argv[1];
	int Nfiles = std::stoi(argv[2]);/* 平均化する線の数 */
	std::ifstream ifs(paths);
	std::string line;

	double*** data_arr = new double** [Nfiles];
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

				std::cout << path << std::endl;
				int Ndata;
				std::string header;
				Nlines = 0;
				Ncolumns = 0;
				double** data = fit::read_result(path, header, Nlines, Ncolumns);
				data_arr[i_file] = data;
				double** running_means = new double* [4];/* 0=3, 1=5, 2=3(log), 3=5(log) */
				running_means[0] = fit::running_mean(Nlines, 3, data[3]);
				running_means[1] = fit::running_mean(Nlines, 5, data[3]);
				double* data_3_log = new double [Nlines];/* データ3列目（フィッティング後）の対数 */
				for(int i=0; i<Nlines; i++){
					data_3_log[i] = std::log10(data[3][i]);
				}
				running_means[2] = fit::running_mean(Nlines, 3, data_3_log);
				running_means[3] = fit::running_mean(Nlines, 5, data_3_log);
				for(int i=0; i<Nlines; i++){
					running_means[2][i] = std::pow(10.0, running_means[2][i]);
					running_means[3][i] = std::pow(10.0, running_means[3][i]);
				}
				
				/* ==== ファイルに保存 ==== */	
				header = "# RUNNING_MEAN\n"+header + "#height observed simulated sim(fitted) sim(running_mean_3) sim(running_mean_5) sim(running_mean_3_log) sim(running_mean_5_log)\n";
				fit::save_data_and_result(path+"_runningmean.dat", header, Nlines, Ncolumns, data, 4, running_means);
				/* ==== */
				/* ^^ */
			}
		}
		i_file++;
	}
	
	double** averaged = new double* [Ncolumns];
	for(int i=0; i<Ncolumns; i++){
		averaged[i] = new double [Nlines];
	}
	double* tmp = new double[Nfiles];
	for(int k=0; k<Nlines; k++){
		for(int i=0; i<Nfiles; i++){
			tmp[i] = data_arr[i][3][k];
		}
		averaged[3][k] = fit::mean(Nfiles, tmp);
	}

	for(int i=0; i<Nlines; i++){
		std::cout << averaged[3][i] << std::endl;
	}

	fit::save_data_and_result("averaged_trials.dat", "#TangentialHeight obs sim sim(fit) sim(averaged)\n", Nlines, Ncolumns, data_arr[0], averaged[3]);
	return 0;
	



	return 0;
}
