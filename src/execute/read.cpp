
#include"execute.h"

double read_mystic_rad(int NN){
	std::string path = (DIR_UVSPEC+"mc"+std::to_string(NN)+".rad");
	std::ifstream ifs(path);

	if(!ifs){
		std::cerr << "Failed to open file '" << path << "'" << std::endl;
		return 0.0;
	}

	std::string line;

	std::getline(ifs, line);
	std::istringstream iss(line);
	std::string token;
	
	int col_index = 0;
	while(iss >> token){
		if(col_index == MYSTIC_RADIANCE_COLUMN){
			return std::stod(token);
		}
		col_index++;
	}
	return 0.0;
}

double read_stdout(std::string path_stdout, int number_of_column){
	std::ifstream ifs(path_stdout);

	if(!ifs){
		std::cerr << "Failed to open file '" << path_stdout << "'" << std::endl;
		return 0.0;
	}

	std::string line;

	std::getline(ifs, line);
	std::istringstream iss(line);
	std::string token;
	
	int col_index = 0;
	while(iss >> token){
		if(col_index == number_of_column){
			return std::stod(token);
		}
		col_index++;
	}
	return 0.0;
}
