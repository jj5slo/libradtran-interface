/*
 * discretize_spectrum.cpp
 *
 *
 */
//元のスペクトルをあたえる
//分割する範囲を指定
//何分割するか
//それぞれの代表値を決定
//重みを決定



int main(int argc, char *argv[]){
	std::string path;
	if (argc == 2){
		path = std::string(argv[1]);
	}
	else{
		std::cerr << "Usage: ./discretize_spectra PATH_SPECTRUM MIN_WL MAX_WL NUMBER_OF_PART" << std::endl;
		return 1;
	}
	std::string header;
	int Nlines;
	int Ncolumns;
	double** spectrum = readwrite::read_data(path, header, Nlines, Ncolumns);


	
	
	return 0;
}
