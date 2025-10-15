
#include"fit.h"


// int main(int argc, char *argv[]){
// 	
// 	if(argc != 2){
// 		std::cerr << "Usage ./fit paths.txt" << std::endl;
// 	}
// 	std::string path = argv[1];
// 
// 	std::string paths = argv[1];
// 	std::ifstream ifs(paths);
// 	std::string line;
// 	
// 	while(std::getline(ifs, line)){
// 		if(line[0] != '#'){
// 			std::istringstream iss_read(line);
// 			std::string token;
// 			while(iss_read >> token){
// 				fitting_result(token);
// 			}
// 		}
// 	}
// 
// 	return 0;
// }


double* fit::fitting_result(int Ndata, double* obs, double* sim, int TYPE){
	double** data = new double [3];
	double* fitted = new double[Ndata];
	data[1] = obs;
	data[2] = sim;

/* ==== フィッティング処理 ==== */
	if(	TYPE == fit::LS ||
			TYPE == fit::LOG_LS){
		double a = 0.0;
		double b = 0.0;
		double* fitting_coefficient = new double[2];
	
	/* == 対数 == */
	/* そのままだと0がマイナス無限大になるので、観測の最小値だけ足してから */
		if(TYPE == fig::LOG_LS){
			double offset = obs[0];
			for(int i=1; i<; i++){
				if(obs[i] < offset){ offset = obs[i]; }
			}
			for(int j=0; j<Ndata; j++){/* オフセットだけ足す */
				data[2][j] = data[2][j] + offset;
			}
			for(int j=0; j<Ndata; j++){/* 対数をとる */
				data[1][j] = std::log10(data[i][j]);
				data[2][j] = std::log10(data[i][j]);
			}
		}
	/* == */

		fitting_coefficient = fit::leastsquare(data, Ndata);
		a = fitting_coefficient[0];
		b = fitting_coefficient[1];

		std::cout << "a: " << a << "\nb: " << b << std::endl;
		for(int i=0; i<Ndata; i++){
			fitted[i] = a*data[2][i] + b;
		}
		
	}
/* ==== */
	return fitted;
}


double* fit::fitting_result(std::string path, int TYPE, int* Ndata){
	
	std::string header;
	int Nlines = 0;
	int Ncolumns = 0;
	double** data = fit::read_result(path, &header, &Nlines, &Ncolumns);
	/*	data[0] : height
			data[1] : x(observed, fit to)
			data[2] : y(simulated, to fit)
	*/
	
	if(Nlines == 0 || Ncolumns == 0){
		std::cerr << "Error: fitting_result: invalid file format." << std::endl;
		return 1;
	}

/* ==== フィッティング処理 ==== */
	double* fitted = new double[Nlines];
	fitted = fit::fitting_result(Nlines, data[1], data[2], TYPE);
/* ==== */

/* ==== ファイルに保存 ==== */	
	header = "TYPE="+std::to_string(TYPE)+"\n" + header;
	save_fitting_result(path, header, data, fitted);
/* ==== */
	Ndata = Nlines;
	return fitted;
}


		


	





