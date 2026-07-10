#include <iostream>
#include <fstream>
#include <string>
#include <vector> // 追加: 複数ファイルを動的に扱うため

#include "readwrite.h"

/* 各要素の全ファイルの平均をとる */

int main(int argc, char* argv[]){
	std::vector<std::string> input_files;
	std::string output_file = "mean.dat"; // デフォルトの出力ファイル名

	// 引数の解析
	for(int i = 1; i < argc; i++){
		std::string arg = argv[i];
		if(arg == "-o"){
			// -o の次の引数を出力ファイル名として取得
			if(i + 1 < argc){
				output_file = argv[++i];
			} else {
				std::cerr << "Error: -o オプションには出力ファイル名が必要です。" << std::endl;
				return 1;
			}
		} else {
			// -o やその指定先以外の引数はすべて入力ファイルとみなす
			input_files.push_back(arg);
		}
	}

	int Nfiles = input_files.size();
	
	// 入力ファイルが1つも指定されていない場合は使い方を表示
	if(Nfiles == 0){
		std::cerr << "Usage: " << argv[0] << " [-o OUTPUT.dat] file1.dat file2.dat ..." << std::endl;
		return 1;
	}

	// --- read_list は廃止し、input_files を直接使用する ---
	
	double*** data_arr = new double** [Nfiles];
	std::string header_0;
	int Nlines;
	int Ncolumns;
	
	data_arr[0] = readwrite::read_data(input_files[0], header_0, Nlines, Ncolumns);
	for(int i=1; i<Nfiles; i++){
		std::string t_header;
		data_arr[i] = readwrite::read_fixed_data(input_files[i], t_header, Nlines, Ncolumns);
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

	// ヘッダーに出力元のファイル数を記録するように変更
	std::string header_msg = "#mean from " + std::to_string(Nfiles) + " files\n";
	readwrite::save_data(output_file, header_msg, Nlines, Ncolumns, means);
	
	return 0;
}
