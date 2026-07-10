#include <iostream>
#include <fstream>
#include <string>

#include "readwrite.h"

/* 各要素の全ファイルの平均をとる */

int main(int argc, char* argv[]){
	std::string PATHS_PATH;
	int Number_of_index_columns;
	if(argc == 2){
		PATHS_PATH = argv[1];
	}
	else{
		std::cerr << "Usage: ./mean PATHs.txt" << std::endl;
		return 1;
	}
	
	int Nfiles = 0;
	std::string* paths = read_list(PATHS_PATH, Nfiles);

	double*** data_arr = new double** [Nfiles];
	std::string header_0;
	int Nlines;
	int Ncolumns;
	data_arr[0] = read_data(paths[0], header_0, Nlines, Ncolumns);
	for(int i=1; i<Nfiles; i++){
		std::string t_header;
		data_arr[i] = read_fixed_data(paths[i], t_header, Nlines, Ncolumns);
	}

	/* data[COL][LINE] */
	/* Allocate_memory すればキャッシュで高速化が期待できる */
	double** means = new double* [Ncolumns];
	for(int i=0; i<Ncolumns; i++){
		means[i] = new double[Nlines];
	}
	for(int i=0; i<Ncolumns; i++){
		for(int j=0; j<Nlines; j++){
			means[i][j] = 0.0;
			for(int k=0; k<Nfiles; k++){
				means[i][j] += data_arr[k][i][j];
			}
			means[i][j] = means[i][j] / (double)Nfiles;
		}
	}
			
	save_data("mean.dat", "#mean from "+PATHS_PATH+"\n", Nlines, Ncolumns, means);
}
		
