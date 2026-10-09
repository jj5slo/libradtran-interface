
#include "atmNair_to_temperature.h"

/*

  TODO iTOP以下だけ温度を求める。
	     それ以上は元のファイルから埋める
 */




int main(int argc, char* argv[]){
	std::string PATH;
	int iTOP;
	double COEF;/* Experiment */
	if(argc == 4){
		PATH       =           argv[1];
		iTOP       = std::stoi(argv[2]);
		COEF       = std::stod(argv[3]);
	}
	else{
		std::cerr << "Usage: ./min_max PATH.txt iTOP(0start) COEF " << std::endl;
		return 1;
	}
		
	int Nheights = 0;
	auto earth = PlanetParam( 6370.e3 );
	auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
	ParamAtmosphere* pAtm = readParamAtmosphere(PATH, Nheights);
	
	double* heights = new double [Nheights];
	double* x = new double[Nheights];

	for(int i=0; i<Nheights; i++){
		heights[i] = pAtm[i].z;
		x[i] = pAtm[i].Nair;
	}
	
	double top_height = pAtm[iTOP].z;

	double pressure_at_top;
//	pressure_at_top = 2 * pAtm[Nheights-1].p;
//		ParamAtmosphere* oldatm = Nair_to_atmosphere_old(Nheights, earth, heights, x, pressure_at_top);
//	pressure_at_top = COEF * std::pow(10, (std::log10(pAtm[iTOP+1].p) + std::log10(pAtm[iTOP].p))/2.0);
//	std::cerr << "p" << iTOP+1 << " " << pAtm[iTOP+1].p << std::endl;
//	std::cerr << "p" << iTOP << "+1/2 " << pressure_at_top << std::endl;
//	std::cerr << "p" << iTOP << " " << pAtm[iTOP].p << std::endl;
//	return 0;

//		ParamAtmosphere* newatm = Nair_to_atmosphere_new(Nheights, earth, heights, x, iTOP, pressure_at_top);
		ParamAtmosphere* newatm = call_temperatures(pAtm, Nheights, iTOP, COEF);
	
	std::string save_path = PATH+"_temp.dat";
	
	saveParamAtmosphere(save_path, newatm, Nheights, 7);
//	std::string header = "# top_height(processed): "+ std::to_string(top_height) + "\n#z N_ref p_ref T_ref  N_n p_n T_n";
//	ofs << header << std::endl;	
//	for(int i=0; i<Nheights; i++){
//			ofs << refatm[i].z <<" "<< refatm[i].Nair <<" "<< refatm[i].p <<" "<< refatm[i].T <<" "<<" "<< oldatm[i].Nair <<" "<< oldatm[i].p <<" "<< oldatm[i].T <<" "<<" "<< newatm[i].Nair <<" "<< newatm[i].p <<" "<< newatm[i].T << std::endl;
//		ofs << refatm[i].z <<" "<< refatm[i].Nair <<" "<< refatm[i].p <<" "<< refatm[i].T <<" "<<" "<< newatm[i].Nair <<" "<< newatm[i].p <<" "<< newatm[i].T << std::endl;
//	}
//	ofs.close();
	return 0;
}
