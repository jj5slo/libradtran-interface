
#include <memory_allocate.h> /* AndoLab */
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include "readwrite.h"

std::string* read_list(std::string path, int &Nelements){
	Nelements = 0;
	std::ifstream t_ifs(path);
	std::string line;
	while(std::getline(t_ifs, line)){
		if(line[0] != '#'){
			std::istringstream iss_read(line);
			std::string token;
			while(iss_read >> token){
				Nelements++;
			}
		}
	}
	t_ifs.close();
	std::ifstream ifs(path);
	line = "";
	int i=0;
	std::string* list = new std::string[Nelements];
	while(std::getline(ifs, line)){
		if(line[0] != '#'){
			std::istringstream iss_read(line);
			std::string token;
			while(iss_read >> token){
				list[i] = token;
				i++;
			}
		}
	}
	return list;
}

double** read_data(std::string path, std::string &header, int &Nlines, int &Ncolumns){/* 参照渡し */
	
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
		if(temp_line[0] != '#' && temp_line != "" && temp_line != " " && temp_line != "\t"){
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
	std::ifstream ifs(path);
	std::string line;
	int line_index = 0;/* データのある行だけを数える */
	col_index = 0;	
	while(std::getline(ifs, line)){
		if(line[0] != '#' && line != "" && line != " " && line != "\t"){
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

double** read_fixed_data(std::string path, std::string &header, int &Nlines, int &Ncolumns){/* 参照渡し */
	
	std::cout << "Reading: " << path << std::endl;
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
	std::ifstream ifs(path);
	std::string line;
	int line_index = 0;/* データのある行だけを数える */
	int col_index = 0;	
	while(std::getline(ifs, line)){
		if(line[0] != '#' && line != "" && line != " " && line != "\t"){
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

void save_data(std::string path, std::string header, int Nlines, int Ncolumns, double** data){	
	std::cout << "Saving to: " << path << std::endl;
	std::ofstream ofs(path);
	ofs << header << "\n";
	for(int i=0; i<Nlines; i++){
		for(int j=0; j<Ncolumns; j++){
			ofs << data[j][i] << " ";
		}
		ofs << std::endl;
	}
	ofs.close();
}
void save_data(std::string path, std::string header, int Nlines, int Ncolumns, int** data){	
	std::cout << "Saving to: " << path << std::endl;
	std::ofstream ofs(path);
	ofs << header << "\n";
	for(int i=0; i<Nlines; i++){
		for(int j=0; j<Ncolumns; j++){
			ofs << data[j][i] << " ";
		}
		ofs << std::endl;
	}
	ofs.close();
}
void save_data_and_string(std::string path, std::string header, int Nlines, int Ncolumns, double** data, std::string* string){	
	std::cout << "Saving to: " << path << std::endl;
	std::ofstream ofs(path);
	ofs << header << "\n";
	for(int i=0; i<Nlines; i++){
		for(int j=0; j<Ncolumns; j++){
			ofs << data[j][i] << " ";
		}
		ofs << string[i];
		ofs << std::endl;
	}
	ofs.close();
}
