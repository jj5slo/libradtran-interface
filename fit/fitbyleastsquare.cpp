// 最小二乗法でフィッティング

#include"fit.h"


int main(int argc, char *argv[]){
	
	if(argc != 2){
		std::cerr << "Usage ./fit paths.txt" << std::endl;
	}
	std::string path = argv[1];

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


int fitting_result(std::string path){

	std::ifstream temp_ifs(path);/* 行数数え */
	if(!temp_ifs){
		std::cerr << "Failed to open file '" << path << "'" << std::endl;
		/* return */
	}
	std::string temp_line;
	int Ndatas = 0;
	while(std::getline(temp_ifs, temp_line)){
		if(temp_line[0] != '#' && temp_line != ""){ Ndatas++; }
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
	double sum_x;
	double sum_y;
	double sum_y2;
	double sum_xy;

	double a{0.0};
	double b{0.0};

//	std::getline(ifs, line);
//	std::istringstream iss_count(line);
//	std::string temp;
//	Ncolumns = 0;
//	while(iss_count >> temp){
//		Ncolumns++;
//	}
//	double *heights = new double[Nheights];
	
	/* データ読み */
	std::ifstream ifs(path);
	std::string line;
	int line_index = 0;
	int data_index = 0;
	int col_index = 0;
	
	sum_xy = 0;
	sum_x = 0;
	sum_y = 0;
	sum_y2 = 0;

	while(std::getline(ifs, line)){
		line_index++;
		
		if(line[0] != '#' && line != ""){
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
	
	for(int i = 0; i < Ndatas; i++){
//		if(h[i] >= 20.0){
			sum_xy += x[i] * y[i];
			sum_x += x[i];
			sum_y += y[i];
			sum_y2 += y2[i];
//		}
	}
	double nd = (double)Ndatas;
	a = (nd*sum_xy - (sum_x*sum_y)) / (nd*sum_y2 - (sum_y*sum_y));
	b = (sum_x - sum_y * a)/nd;
	if((nd*sum_y2 - (sum_y*sum_y)) == 0.0){
		a = 1.0;
		b = sum_x / nd;
	}

	ifs.close();
	std::cout << sum_xy <<" "<< sum_x << " "<<sum_y<<" "<<sum_y2<<std::endl;
	std::cout << "a; " << a << "\nb; " << b << std::endl;

	std::ofstream ofs(path+".fittedFullLS");
	ofs << comment << "\n";
	for(int i=0; i<Ndatas; i++){
		ofs << h[i] << " " << x[i] << " " << y[i] << " " << a*y[i]+b << "\n";
	}

	ofs.close();
	return 0;
}


		


	





