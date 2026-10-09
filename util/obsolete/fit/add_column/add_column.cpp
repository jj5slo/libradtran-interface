#include "../fit.h"

int main(int argc, char* argv[]){
	if(argc != 4){
		std::cerr << "Usage ./fit paths.txt adding.txt N" << std::endl;
		return 1;
	}

	std::string paths = argv[1];
	std::string path_add = argv[2];
	int Nfiles = std::stoi(argv[3]);/* 平均化する線の数 */

	int Nadding_lines = 0;
	int Nadding_columns = 0;
	std::string adding_header;
	double ** adding = fit::read_result(path_add, adding_header, Nadding_lines, Nadding_columns);
	
	std::ifstream ifs(paths);
	std::string line;
	while(std::getline(ifs, line)){
		if(line[0] != '#'){
			std::istringstream iss_read(line);
			std::string token;

			while(iss_read >> token){
				std::string path = token;
				std::string header;
				int Nlines = 0;
				int Ncolumns = 0;
				double** data = fit::read_result(path, header, Nlines, Ncolumns);
				fit::save_data_and_result(path, header+"#1height 2observed 3simulated 4sim(fitted) 5sim(running_mean_3) 6sim(running_mean_5) 7sim(running_mean_3_log) 8sim(running_mean_5_log) 9height 10averaged\n", Nlines, Ncolumns, data, Nadding_columns, adding);
			}
		}
	}

	return 0;
}
