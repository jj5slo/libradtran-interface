/*
 *  fit.cpp
 */

#include"fit.h"

int main(int argc, char *argv[]){
//	std::string paths;
//	std::string path;
//	std::getline(std::cin, paths);
//	std::istringstream iss(paths);
//	while(iss >> path){
//		std::cout << path << std::endl;
//		fitting_result(path);
//	}
//	return 0;


	if(argc != 2){
		std::cerr << "Usage ./fit paths.txt" << std::endl;
	}
	std::string paths = argv[1];
	std::ifstream ifs(paths);
	std::string line;
	
	while(std::getline(ifs, line)){
		if(line[0] != '#'){
			std::istringstream iss_read(line);
			std::string token;
			while(iss_read >> token){
				fitting_result(token);
			}
		}
	}

	return 0;
}



int fitting_result(std::string path){ /*saveで作ったファイルから読み込む */

	std::ifstream temp_ifs(path);/* 行数数え */
	if(!temp_ifs){
		std::cerr << "Failed to open file '" << path << "'" << std::endl;
		/* return */
	}
	std::string temp_line;
	int Ndatas = 0;
	while(std::getline(temp_ifs, temp_line)){
		if(temp_line[0] != '#'){ Ndatas++; }
	}
	temp_ifs.close();

	std::string comment = "";

	double *h = new double[Ndatas];/* height */
	for(int i=0; i<Ndatas; i++){ h[i] = 0; }
	double *x = new double[Ndatas];
	for(int i=0; i<Ndatas; i++){ x[i] = 0; }
	double *y = new double[Ndatas];
	for(int i=0; i<Ndatas; i++){ y[i] = 0; }
	double *y2 = new double[Ndatas];/* y2乗 */
	for(int i=0; i<Ndatas; i++){ y2[i] = 0; }
	
	/* データ読み */
	std::ifstream ifs(path);
	std::string line;
	int line_index = 0;
	int data_index = 0;
	int col_index = 0;
	
	while(std::getline(ifs, line)){
		line_index++;
		
		if(line[0] != '#'){
			std::istringstream iss_read(line);
			std::string token;
			col_index = 0;

			while(iss_read >> token){
				if(col_index == 0 ){
					h[data_index] = std::stod(token);
				}
				if(col_index == 1 ){
					x[data_index] = std::stod(token);
				}
				if(col_index == 2 ){
					y[data_index] = std::stod(token);
					y2[data_index] = y[data_index]*y[data_index];
				}	
				col_index++;
			}
			data_index++;
		}
		else comment += line + "\n";
	}

	std::pair<double*, double*> y_minmax = std::minmax_element(y, y+Ndatas);
	double* y_min = y_minmax.first;
	double* y_max = y_minmax.second;
	int y_min_i = y_min - y;
	int y_max_i = y_max - y;
	double x_rminv = x[y_min_i];
	double x_rmaxv = x[y_max_i];

	double scaling_factor = (x_rmaxv - x_rminv) / (*y_max - *y_min);

	std::cout << "y_min y_max y_min_i y_max_i x_rminv x_rmaxv scaling_factor\n" << y_min <<" "<< y_max <<" "<< y_min_i <<" "<< y_max_i <<" "<< x_rminv <<" "<< x_rmaxv <<" "<< scaling_factor << std::endl;

	double* y_fitted = new double[Ndatas];
	for(int i=0; i<Ndatas; i++){
		y_fitted[i] = y[i] * scaling_factor + (x_rminv - *y_min) ;
	}
		


	std::ofstream ofs(path+".fitted");

	if(!ofs){
		std::cerr << "Failed to open file '" << path << "'" << std::endl;
		return 1;
	}
	ofs << comment << "\n";
	ofs << "#y_min y_max y_min_i y_max_i x_rminv x_rmaxv scaling_factor\n#" << y_min <<" "<< y_max <<" "<< y_min_i <<" "<< y_max_i <<" "<< x_rminv <<" "<< x_rmaxv <<" "<< scaling_factor << "\n";
	ofs << "#height observed simulated simulated-fitted\n";

	for(int i=0; i<Ndatas; i++){
		ofs << h[i] << " " << x[i] << " " << y[i] << " " << y_fitted[i] << "\n";
	}

	return 0;
}


