#include "execute.h"


std::string obs_path(std::string data_dir, obsDateTime dt){
	std::ostringstream filename;
	filename << "h08_b01_s01s02_" << std::setw('4') << std::setfill('0') << dt.Year() << std::setw('2') << std::setfill('0') << dt.Month() <<  std::setw('2') << std::setfill('0') << dt.Date() << "_" <<   std::setw('2') << std::setfill('0') << dt.Hour() <<  std::setw('2') << std::setfill('0') << dt.Minute() <<  std::setw('2') << std::setfill('0') << dt.Second();
	std::string path = data_dir + filename.str();
	return path;
}

Observed* read_obs(int *aNobs, std::string path_obs ){
	int line_index;
	int Nlines = 0;
	int Nobs = 0;
	int col_index;
	int Ncolumns = 0;
	int Nheights;
	int obs_index;
	int data_index;
	double lat = 0.0;
	double lon = 0.0;


	std::ifstream temp_ifs(path_obs);/* 行数数え */
	if(!temp_ifs){
		std::cerr << "Failed to open file '" << path_obs << "'" << std::endl;
		/* return */
	}
	std::string temp_line;
	while(std::getline(temp_ifs, temp_line)){
		Nlines++;
	}
	temp_ifs.close();
	
	Nobs = Nlines - 2;/* height の行 2行は要らない */
	Observed *obsds = new Observed[Nobs];

	std::ifstream ifs(path_obs);
	
	std::string line;
	line_index = 0;
	std::getline(ifs, line);/* 読み飛ばし */
	line_index++;
	
	std::getline(ifs, line);
	std::istringstream iss_count(line);
	std::string temp;
	Ncolumns = 0;
	while(iss_count >> temp){
		Ncolumns++;
	}
	Nheights = Ncolumns - 2;/* lat, lon の分はいらない */
	double *heights = new double[Nheights];

	std::istringstream iss_read(line);
	std::string token;
	col_index = 0;
	int height_index = 0;
	while(iss_read >> token){
		if(col_index >= 2 && height_index < Nheights){
			heights[height_index] = std::stod(token);
			height_index++;
		}
		col_index++;
	}
	line_index++;


	double *data = new double[Nheights];	
	obs_index = 0;


	while(std::getline(ifs, line)){
		std::istringstream iss(line);
		std::string token;
		
		col_index = 0;
		data_index = 0;
		while(iss >> token){
			if(col_index == 0){ lat = std::stod(token); }
			if(col_index == 1){ lon = std::stod(token); }
			if(col_index >= 2 && token != "\n" && data_index < Nheights){
				data[data_index] = std::stod(token);
				data_index++;
			}
			col_index++;
		}
		obsds[obs_index].set(lat, lon, Nheights, heights, data);
		obs_index++;
		line_index++;
	}

	delete[] heights;
	delete[] data;
	
	*aNobs = Nobs;
	return obsds;
}
