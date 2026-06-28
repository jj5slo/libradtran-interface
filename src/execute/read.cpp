
#include <regex>

#include "execute.h"
#include "filematch.h"

double read_mystic_rad(const std::string DIR_UVSPEC, int NN){
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

double read_mystic_rad_NN(const std::string DIR_UVSPEC){
	std::string* paths_list = nullptr;
	int Nfiles = 0;
	listMatchingFiles(DIR_UVSPEC, std::regex(R"(mc.*\.rad)"), paths_list, Nfiles);

	if(Nfiles != 1){ throw std::runtime_error("read_mystic_rad_NN: number of files is not 1!!"); }/* ERROR */
	
	std::string path = DIR_UVSPEC+paths_list[0];

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
double read_mystic_rad_sd(const std::string DIR_UVSPEC){
	std::string* paths_list = nullptr;
	int Nfiles = 0;
	listMatchingFiles(DIR_UVSPEC, std::regex(R"(mc.*\.rad\.std)"), paths_list, Nfiles);

	if(Nfiles != 1){ throw std::runtime_error("read_mystic_rad_NN: number of files is not 1!!"); }/* ERROR */
	
	std::string path = DIR_UVSPEC+paths_list[0];

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

double read_mystic_rad_spc(const std::string DIR_UVSPEC){
	std::string path = (DIR_UVSPEC+"mc.rad.spc");
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
		if(col_index == 4){/* wl ix iy iz rad */
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

