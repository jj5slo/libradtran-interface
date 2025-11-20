
#include<memory_allocate.h> /* AndoLab */
#include<iostream>
#include "fit.h"

double** fit::read_result(std::string path, std::string &header, int &Nlines, int &Ncolumns){/* 参照渡し */
	
	std::cout << "Reading: " << path << std::endl;
	std::ifstream temp_ifs(path);/* 行列数数え */
	if(!temp_ifs){
		std::cerr << "Failed to open file '" << path << "'" << std::endl;
		/* return */
	}
	std::string temp_line;
	Nlines = 0;
	Ncolumns = 0;
	int col_index = 0;
	while(std::getline(temp_ifs, temp_line)){
		if(temp_line[0] != '#' && temp_line != ""){
			Nlines++;
			std::istringstream iss_read(temp_line);
			std::string token;
			col_index = 0;
			while(iss_read >> token){
				col_index++;
			}
			if(Ncolumns < col_index){
				Ncolumns = col_index;
			}
		}
	}
	temp_ifs.close();

/* ==== 初期化 ==== */
	header = "";
	double **data = AndoLab::allocate_memory2d(Ncolumns, Nlines, 0.0);
	std::cout << Ncolumns << " x " << Nlines << std::endl;
/*
	double **data = new double*[ptr_Ncolumns];
	data[0] = new double*[ptr_NcolumnsNlines];
	for(int i=0; i<ptr_Ncolumns; i++){
		data[i] = data[0] + iNlines;
		for(int j=0; j<ptr_Ncolumns; j++){
			data[i][j] = 0.0;
		}
	}
*/
/* ==== */

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
	int line_index = 0;/* データのある行だけを数える */
	col_index = 0;
	
	while(std::getline(ifs, line)){
		if(line[0] != '#' && line != ""){
			std::istringstream iss_read(line);
			std::string token;
			col_index = 0;

			while(iss_read >> token){
				data[col_index][line_index] = std::stod(token);
				col_index++;
			}
			line_index++;
		}
		else header = std::string( header + line + "\n" );/* コメントが全てヘッダに集約される(仕様) */
	}
	ifs.close();
	return data;/* don't forget deallocation */
}



void fit::save_fitting_result(std::string path, std::string header, int Nlines, double** data, double* fitted){	
	std::cout << "Saving to: " << path << std::endl;
	std::ofstream ofs(path);
	ofs << header << "\n";
	for(int i=0; i<Nlines; i++){
		ofs << data[0][i] << " " << data[1][i] << " " << data[2][i] << " " << fitted[i] << "\n";
	}
	ofs.close();
}
void fit::save_data_and_result(std::string path, std::string header, int Nlines, double* data, double* result){	
	std::cout << "Saving to: " << path << std::endl;
	std::ofstream ofs(path);
	ofs << header << "\n";
	for(int i=0; i<Nlines; i++){
		ofs << data[i] << " ";
		ofs << result[i] << "\n";
	}
	ofs.close();
}
void fit::save_data_and_result(std::string path, std::string header, int Nlines, int Ncolumns, double** data, double* result){	
	std::cout << "Saving to: " << path << std::endl;
	std::ofstream ofs(path);
	ofs << header << "\n";
	for(int i=0; i<Nlines; i++){
		for(int j=0; j<Ncolumns; j++){
			ofs << data[j][i] << " ";
		}
		ofs << result[i] << "\n";
	}
	ofs.close();
}
void fit::save_data_and_result(std::string path, std::string header, int Nlines, int Ncolumns, double** data, int Nrc, double** result){	
	std::cout << "Saving to: " << path << std::endl;
	std::ofstream ofs(path);
	ofs << header << "\n";
	for(int i=0; i<Nlines; i++){
		for(int j=0; j<Ncolumns; j++){
			ofs << data[j][i] << " ";
		}
		for(int j=0; j<Nrc; j++){
			ofs << result[j][i] << " ";
		}
		ofs << "\n";
	}
	ofs.close();
}
void fit::save_data(std::string path, std::string header, int Nlines, int Ncolumns, double** data){	
	std::cout << "Saving to: " << path << std::endl;
	std::ofstream ofs(path);
	ofs << header << "\n";
	for(int i=0; i<Nlines; i++){
		for(int j=0; j<Ncolumns; j++){
			ofs << data[j][i] << " ";
		}
		ofs << "\n";
	}
	ofs.close();
}
