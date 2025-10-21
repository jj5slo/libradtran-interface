
#include<memory_allocate.h> /* AndoLab */

#include "fit_result.h"

double** fit::read_result(std::string path, std::string *header, int *ptr_Nlines, int *ptr_Ncolumns){/* 参照渡しのほうがよい */
	
	std::ifstream temp_ifs(path);/* 行列数数え */
	if(!temp_ifs){
		std::cerr << "Failed to open file '" << path << "'" << std::endl;
		/* return */
	}
	std::string temp_line;
	*ptr_Nlines = 0;
	*ptr_Ncolumns = 0;
	int col_index = 0;
	while(std::getline(temp_ifs, temp_line)){
		if(temp_line[0] != '#' && temp_line != ""){
			*ptr_Nlines++;
			std::istringstream iss_read(temp_line);
			std::string token;
			col_index = 0;
			while(iss_read >> token){
				col_index++;
			}
			if(*ptr_Ncolumns < col_index){
				*ptr_Ncolumns = col_index;
			}
		}
	}
	temp_ifs.close();

/* ==== 初期化 ==== */
	*header = "";
	double **data = AndoLab::allocate_memory2d(*ptr_Ncolumns, *ptr_Nlines, 0.0);
	
/*
	double **data = new double*[ptr_Ncolumns];
	data[0] = new double*[ptr_Ncolumns*ptr_Nlines];
	for(int i=0; i<ptr_Ncolumns; i++){
		data[i] = data[0] + i*ptr_Nlines;
		for(int j=0; j<ptr_Ncolumns; j++){
			data[i][j] = 0.0;
		}
	}
*/
/* ==== */

//	std::getline(ifs, line);
//	std::istringstream iss_count(line);
//	std::string temp;
//	*ptr_Ncolumns = 0;
//	while(iss_count >> temp){
//		*ptr_Ncolumns++;
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
		else *header = std::string( *header + line + "\n" );/* コメントが全てヘッダに集約される(仕様) */
	}
	ifs.close();
	return data;/* don't forget deallocation */
}



void save_fitting_result(std::string path, std::string header, int Nlines, double** data, double* fitted){	
	std::ofstream ofs(path+".fitted");
	ofs << header << "\n";
	for(int i=0; i<Nlines; i++){
		ofs << data[0][i] << " " << data[1][i] << " " << data[2][i] << " " << fitted[i] << "\n";
	}
	ofs.close();

}
