#include "fit.h"

int main(int argc, char* argv[]){
	double bottom_height = 29.5;
	double top_height = 60.5;
	if(argc != 4){
		std::cerr << "Usage ./fit paths.txt OBS SIM" << std::endl;
		return 1;
	}
	constexpr int TYPE {fit::LOG};

	std::string path;

	std::string paths = argv[1];
	int OBS_COL = std::stod(argv[2]);
	int SIM_COL = std::stod(argv[3]);
	std::ifstream ifs(paths);
	std::string line;

//	double* a_offset = new double[2];
//	
//	std::getline(ifs, line);
//		if(line[0] != '#'){
//			std::istringstream iss_read(line);
//			std::string token;
//			while(iss_read >> token){
//				path = token;
//				std::cerr << path << std::endl;
//				std::string header;
//				int Nlines = 0;
//				int Ncolumns = 0;
//				double** data = fit::read_result(path, header, Nlines, Ncolumns);
//	
//				double offset = fit::mean(Nlines, data, 95.0, 100.0);/* data[1] は 観測値 */
//				std::cout << offset << std::endl;
//
//				a_offset = fit::fitting_result(Nlines, data[0], data[1], data[2], 20.0, 60.0, offset, TYPE);
//				std::cout << "a = " << std::to_string(a_offset[0]) <<"\noffset = " << std::to_string(a_offset[1]) << std::endl;
//
//				double* fitted = fit::apply_fitting(Nlines, data[2], a_offset);/* data[2] はシミュレーション値 */
//				
//				for(int i = 0; i<Nlines; i++){
//					std::cout << fitted[i] << " ";
//				}
//				std::cout << std::endl;
//
//				header = "# TYPE="+std::to_string(TYPE)+"\n# FITTING_COEFFICIENT: A="+std::to_string(a_offset[0])+", OFFSET="+std::to_string(a_offset[1])+"\n" + header + "#height observed simulated sim(fitted)\n";
//				fit::save_fitting_result(path, header, Nlines, data, fitted);
//			}
//		}
//
	while(std::getline(ifs, line)){
		if(line[0] != '#'){
			std::istringstream iss_read(line);
			std::string token;
			while(iss_read >> token){
				path = token;

				/* -- */

				std::cerr << path << std::endl;
				int Ndata;
				std::string header;
				int Nlines = 0;
				int Ncolumns = 0;
				double** data = fit::read_result(path, header, Nlines, Ncolumns);
	
				double offset = fit::mean(Nlines, data, 95.0, 100.0);/* data[1] は 観測値 */
				std::cout << offset << std::endl;

				double* a_offset = fit::fitting_result(Nlines, data[0], data[OBS_COL], data[SIM_COL], bottom_height, top_height, offset, TYPE);
				std::cout << "a = " << std::to_string(a_offset[0]) <<"\noffset = " << std::to_string(a_offset[1]) << std::endl;

				double* fitted = fit::apply_fitting(Nlines, data[SIM_COL], a_offset);/* data[2] はシミュレーション値 */
				
//				std::cerr << path << std::endl;
//				std::string header;
//				int Nlines = 0;
//				int Ncolumns = 0;
//				double** data = fit::read_result(path, header, Nlines, Ncolumns);
//	
//				std::cout << "a = " << std::to_string(a_offset[0]) <<"\noffset = " << std::to_string(a_offset[1]) << std::endl;
//
//				double* fitted = fit::apply_fitting(Nlines, data[2], a_offset);/* data[2] はシミュレーション値 */
				
				for(int i = 0; i<Nlines; i++){
					std::cout << fitted[i] << " ";
				}
				std::cout << std::endl;

				/* ==== ファイルに保存 ==== */	
				header = "# TYPE="+std::to_string(TYPE)+"\n#top="+std::to_string(top_height)+", bottom="+std::to_string(bottom_height)+"# FITTING_COEFFICIENT: A="+std::to_string(a_offset[0])+", OFFSET="+std::to_string(a_offset[1])+"\n" + header + "#height observed simulated sim(fitted)\n";
//				header = "# FITTED FOR OTHER DATA\n# TYPE="+std::to_string(TYPE)+"\n# FITTING_COEFFICIENT: A="+std::to_string(a_offset[0])+", OFFSET="+std::to_string(a_offset[1])+"\n" + header + "#height observed simulated sim(fitted)\n";
				fit::save_data_and_result(path+"_fitted.dat", header, Nlines, Ncolumns, data, fitted);
				/* ==== */
				/* ^^ */
			}
		}
	}

	return 0;
}
