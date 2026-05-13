#include "count_to_radiance.h"
#include "readwrite.h"
#include "linear_interpolation.h"

double* count_to_radiance(double* data, int Ndata, double** table, int Nlines){
	double* retdata = new double [Ndata];
		for(int i=0; i<Ndata; i++){
			retdata[i] = linear_interpolation(table, Nlines, data[i]);
		}
	return retdata;
}

int main(int argc, char* argv[]){
	std::string IN_FILEs_LIST;
	std::string OUT_FILEs_LIST;
	int band;
	if(argc == 4){
		IN_FILEs_LIST = std::string(argv[1]);
		OUT_FILEs_LIST = std::string(argv[2]);
		band           = std::stoi(argv[3]);
	}
	else{
		std::cerr << "Usage: ./count_to_rad IN_FILEs_LIST OUT_FILEs_LIST band" << std::endl;
		return 0;
	}

	double** interpolation_table = new double* [2];
	int Nlines;// table
	if(band == 1){
		std::string header;
		int Ncolumns;//table
		double** table = readwrite::read_data("/lhome/sano2/SANO/research/estimate-profile/ObsEquip/Himawari-AHI/himawari_AHI_HSD_b01.dat", header, Nlines, Ncolumns);
		if(Ncolumns != 3){
			std::cerr << "ERROR! conversion table format" << std::endl;
			return 1;
		}
		interpolation_table[0] = table[0];
		interpolation_table[1] = table[2];// radiance
	}
	else{
		std::cerr << "count_to_radiance: ERROR!" << std::endl;
	}
	
	int Nelements;
	int Nelements_out;
	std::string* files = readwrite::read_list(IN_FILEs_LIST, Nelements);
	std::string* outfiles = readwrite::read_list(OUT_FILEs_LIST, Nelements_out);
	if(Nelements != Nelements_out){ std::cout << "ERROR IN != OUT" << std::endl; return 1; }
	for(int elem = 0; elem < Nelements; elem++){
		std::string IN_FILE = files[elem];
		std::string OUT_FILE = outfiles[elem];

		std::string header;
		double** data = readwrite::read_fixed_data_with_skip(IN_FILE, header, 88, 103, 2, 0);
	
		double** converted_data = new double* [103];
		converted_data[0] = data[0];
		converted_data[1] = data[1];
		for(int i=2; i<103; i++){
			converted_data[i] = count_to_radiance(data[i], 88, interpolation_table, Nlines);
			if(converted_data[i] == nullptr){ std::cerr << "ERROR!" << std::endl; return 1; }
		}
		
		header = "            height \nlat   lon   000.0 001.0 002.0 003.0 004.0 005.0 006.0 007.0 008.0 009.0 010.0 011.0 012.0 013.0 014.0 015.0 016.0 017.0 018.0 019.0 020.0 021.0 022.0 023.0 024.0 025.0 026.0 027.0 028.0 029.0 030.0 031.0 032.0 033.0 034.0 035.0 036.0 037.0 038.0 039.0 040.0 041.0 042.0 043.0 044.0 045.0 046.0 047.0 048.0 049.0 050.0 051.0 052.0 053.0 054.0 055.0 056.0 057.0 058.0 059.0 060.0 061.0 062.0 063.0 064.0 065.0 066.0 067.0 068.0 069.0 070.0 071.0 072.0 073.0 074.0 075.0 076.0 077.0 078.0 079.0 080.0 081.0 082.0 083.0 084.0 085.0 086.0 087.0 088.0 089.0 090.0 091.0 092.0 093.0 094.0 095.0 096.0 097.0 098.0 099.0 100.0 ";
		readwrite::save_data(OUT_FILE, header, 88, 103, converted_data);
	}
	return 0;
}

