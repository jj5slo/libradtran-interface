#include "stat.h"

int main(int argc, char* argv[]){
	if(argc != 3){
		std::cerr << "Usage ./fit paths.txt merged_data.dat" << std::endl;
		return 1;
	}
	std::string path;

	std::string paths = argv[1];
	std::string merged_path = argv[2];
	
	std::ifstream temp_ifs(paths);/* 行列数数え */
	if(!temp_ifs){
		std::cerr << "Failed to open file '" << path << "'" << std::endl;
		/* return */
	}
	std::string temp_line;
	int Nlines = 0;
	while(std::getline(temp_ifs, temp_line)){
		if(temp_line[0] != '#' && temp_line != ""){
			Nlines++;
		}
	}
	temp_ifs.close();
	
	double*** datas = new double** [Nlines];/* 全データ格納 */

	std::ifstream ifs(paths);
	std::string line;
	std::string header;
	
	int Ndata;
	int Ncolumns;

	int reading_index=0;
	while(std::getline(ifs, line)){
		if(line[0] != '#'){
			std::istringstream iss_read(line);
			std::string token;
			while(iss_read >> token){
				path = token;

				/* -- */
				std::cerr << path << std::endl;
				datas[reading_index] = read_result(path, header, Ndata, Ncolumns);
				for(int j=0; j<Ndata; j++){
					for(int k=0; k<Ncolumns; k++){
						std::cout << std::to_string(datas[reading_index][k][j]) << " ";
					}
					std::cout << std::endl;
				}
				reading_index++;
				/* -- */
			}
		}
	}
	double* index = new double [Ndata];
	for(int j=0; j<Ndata; j++){
		index[j] = datas[0][0][j];
		std::cout << index[j] << " ";
	}
	std::cout << std::endl;
	std::cout << Nlines <<" "<< Ncolumns <<" "<< Ndata << std::endl;
	double** merged_data = new double* [Nlines];
	for(int i=0; i<Nlines; i++){
		merged_data[i] = new double [Ndata];
		for(int j=0; j<Ndata; j++){
			if(datas[i][0][j] == index[j]){/* 無駄な確認のため遅い */
				merged_data[i][j] = datas[i][2][j];
				std::cout << merged_data[i][j] << " ";
			}
		}
		std::cout << std::endl;
	}
	save_merged_result(merged_path, "# index data", Ndata, Nlines, index, merged_data);


	return 0;
}
