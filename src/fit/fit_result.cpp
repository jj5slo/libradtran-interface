
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


double* fit::fitting_result(int Ndata, double* height, double* obs, double* sim, double min_height, double max_height, double offset, int TYPE){/* TODO MIN, MAX を実際に考慮 */
	double** data = new double* [3];
	double* fitted = new double[Ndata];
	data[0] = height;
	data[1] = obs;
	data[2] = sim;

/* ==== フィッティング処理 ==== */
/* == 最小二乗 == */
	if(	TYPE == fit::LS){
		double a = 0.0;
		double b = 0.0;
		double* fitting_coefficient = new double[2];
	
		fitting_coefficient = fit::leastsquare(data, Ndata, min_height, max_height);
		a = fitting_coefficient[0];
		b = fitting_coefficient[1];

		std::cout << "fitting_result:\n\ta: " << a << "\n\tb: " << b << std::endl;
		for(int i=0; i<Ndata; i++){
			fitted[i] = a*data[2][i] + b;
		}
	}
/* == */
/* == LS_OFFSET == */
	if( TYPE == fit::LS_OFFSET ){
		double Sy2{0.0};
		double Sy{0.0};
		double Sxy{0.0};
		for(int i=0; i<Ndata; i++){
			Sy2 += data[2][i]*data[2][i];
			Sy += data[2][i];
			Sxy += data[1][i]*data[2][i];
		}
		double a = (Sxy - offset*Sy)/Sy2;
		std::cout << "fitting_result:\n\ta: " << a << "\n\toffset: " << offset << std::endl;
		for(int i=0; i<Ndata; i++){
			fitted[i] = a*data[2][i] + offset;
		}
	}	
/* == */
/* == 対数 == */
	if(TYPE == fit::LOG){
		nlopt::opt opt( nlopt::LN_NELDERMEAD, 1 );
		
		fit::SLE_Param slep;
		slep.number_of_iteration = 0;
		slep.data = data;
		slep.Ndata = Ndata;
		slep.offset = offset;
		slep.min_height = min_height;
		slep.max_height = max_height;

		opt.set_min_objective( fit::square_log_error, (void*)(&slep) ); 
		opt.set_xtol_rel(1.0e-6);/* TODO */
		std::vector<double> x(1, 5.0e3);/* 初期値 */
		double minf;

		try {
			slep.number_of_iteration = 0;
			nlopt::result result = opt.optimize(x, minf);
		} catch (std::exception &e){
			std::cout << "NLopt for obtaining fitting coefficient failed. : " << e.what() << std::endl;
		}
		double a = x[0];
		std::cout << "fitting_result:\n\ta: " << a << "\n\toffset: " << offset << "\n\tminf: " << minf << std::endl;
		for(int i=0; i<Ndata; i++){
			fitted[i] = a*data[2][i] + offset;
		}
	}
/* == */
/* ==== */
	return fitted;
}


double* fit::fitting_result(std::string path, double min_height, double max_height, double offset, int TYPE, int* Ndata){
	
	std::string header;
	int Nlines = 0;
	int Ncolumns = 0;
	double** data = fit::read_result(path, header, Nlines, Ncolumns);
	/*	data[0] : height
			data[1] : x(observed, fit to)
			data[2] : y(simulated, to fit)
	*/
	
	if(Nlines == 0 || Ncolumns == 0){
		std::cerr << "Error: fitting_result: invalid file format." << std::endl;
	}

/* ==== フィッティング処理 ==== */
	double* fitted = new double[Nlines];
	fitted = fit::fitting_result(Nlines, data[0], data[1], data[2], min_height, max_height, offset, TYPE);
/* ==== */

/* ==== ファイルに保存 ==== */	
	header = "# TYPE="+std::to_string(TYPE)+"\n" + header;
	fit::save_fitting_result(path, header, Nlines, data, fitted);
/* ==== */
	*Ndata = Nlines;
	return fitted;
}


		


	





