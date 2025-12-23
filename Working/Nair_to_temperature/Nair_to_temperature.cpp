#include <iostream>
#include <fstream>

#include "interface.h"
#include "wrapper.h"

#include "readwrite.h"
constexpr double ATM_MOL_W {28.9408}; /* MSIS */

ParamAtmosphere* Nair_to_atmosphere_new(
	int atm_Nheights,
	obsDateTime dt,
	Geocoordinate*& coord,
	PlanetParam earth,
	double* x,
	const double PRESSURE_AT_TOA_MID//,
//	double* gm_e_arr
);
ParamAtmosphere* Nair_to_atmosphere_old(
	int atm_Nheights,
	obsDateTime dt,
	Geocoordinate*& coord,
	PlanetParam earth,
	double* x,
	const double PRESSURE_AT_TOA_MID//,
//	double* gm_e_arr
);

int main(int argc, char* argv[]){
	std::string PATHS_PATH;
	if(argc == 2){
		PATHS_PATH = argv[1];
	}
	else{
		std::cerr << "Usage: ./min_max PATHs.txt" << std::endl;
		return 1;
	}
	
	int Nfiles = 0;
	std::string* paths = read_list(PATHS_PATH, Nfiles);
	for(int file_i = 0; file_i < Nfiles; file_i++){
		auto earth = PlanetParam( 6370.e3 );
		auto himawari = SatelliteParam( 35790.e3 + earth.radius(), 0.0, 140.7 );
		int Nheights = 0;
		ParamAtmosphere* pAtm = readParamAtmosphere(paths[file_i], Nheights);
		obsDateTime dt(2022, 6, 1, 3, 0, 0);
		Geocoordinate on_ground(earth, himawari, 73.0, 82.0, 0.0);/* 観測データにある緯度経度の高度0km 地点のGeocoordinate */
		double ld_alpha = on_ground.alpha()*Rad2deg;
		Geocoordinate *tparr = new Geocoordinate[Nheights];
		LookingDirection ld;
		for(int i=0; i<Nheights; i++){
			ld.set( ld_alpha, pAtm[i].z/m2km );/* 見る場所決め */
			tparr[i] = ld.tangential_point( earth, himawari);/* tangential point の配列 */
		}
	
		double* x = new double[Nheights];
		for(int i=0; i<Nheights; i++){
			x[i] = pAtm[i].Nair;
		}
		ParamAtmosphere* msisatm = get_msis(dt, tparr, Nheights);
		ParamAtmosphere* oldatm = Nair_to_atmosphere_old(Nheights, dt, tparr, earth, x, pAtm[Nheights-1].p);
		ParamAtmosphere* newatm = Nair_to_atmosphere_new(Nheights, dt, tparr, earth, x, (pAtm[Nheights-1].p + pAtm[Nheights-2].p)/2.0);
		
		std::ofstream ofs(paths[file_i]+"_temp.dat");

		std::string header = "#z N_msis p_msis T_msis  N_o p_o T_o  N_n p_n T_n";
		ofs << header << std::endl;	
		for(int i=0; i<Nheights; i++){
			ofs << msisatm[i].z <<" "<< msisatm[i].Nair <<" "<< msisatm[i].p <<" "<< msisatm[i].T <<" "<<" "<< oldatm[i].Nair <<" "<< oldatm[i].p <<" "<< oldatm[i].T <<" "<<" "<< newatm[i].Nair <<" "<< newatm[i].p <<" "<< newatm[i].T << std::endl;
		}
		ofs.close();
	}
	return 0;
}

ParamAtmosphere* Nair_to_atmosphere_new(
	int atm_Nheights,
	obsDateTime dt,
	Geocoordinate*& coord,
	PlanetParam earth,
	double* x,
	const double PRESSURE_AT_TOA_MID /* TOA - Dz/2 */
){
	constexpr double GM_E {3.986004356e14};/* 理科年表プレミアム 2025 */
	constexpr double BOLTZMANN_CONSTANT { 1.380649e-23 };
	constexpr double NA {6.02214076e23};
	constexpr double GAS_CONSTANT = NA*BOLTZMANN_CONSTANT;

	ParamAtmosphere* atm = new ParamAtmosphere[atm_Nheights];/* will be returned*/
	for(int i=0; i<atm_Nheights; i++){
		atm[i].z = coord[i].altitude() * m2km;
		atm[i].Nair = x[i];/* これが主 */
		if(atm[i].z < 0){ atm[i].z = 0.0; }/* 誤差対策（たぶんいらない） */
	}
//	atm[atm_Nheights-1].p = PRESSURE_AT_TOA;/* TOAではほぼ0気圧 */

	double* pressure_at_midpoint = new double[atm_Nheights - 1];
	double* pressure_at_midpoint_log = new double[atm_Nheights - 1];
	pressure_at_midpoint[atm_Nheights - 2] = PRESSURE_AT_TOA_MID;
	pressure_at_midpoint_log[atm_Nheights - 2] = std::log(PRESSURE_AT_TOA_MID);
	for(int i=atm_Nheights-3; 0<=i; i--){
		double radius = earth.radius() + coord[i].altitude();/* [m] */
		double graviational_acceleration = GM_E / radius / radius;
		double Dz = atm[i+2].z - atm[i+1].z;/* TODO */
		
		pressure_at_midpoint[i] 
			= pressure_at_midpoint[i+1] 
				+ ATM_MOL_W * atm[i+1].Nair * graviational_acceleration * Dz;
		pressure_at_midpoint_log[i] = std::log(pressure_at_midpoint[i]);
	}

	for(int i=1; i<atm_Nheights-1; i++){
		double radius = earth.radius() + coord[i].altitude();/* [m] */
		double graviational_acceleration = GM_E / radius / radius;
		double Dz = atm[i].z - atm[i-1].z;
		atm[i].T = - (ATM_MOL_W * graviational_acceleration * Dz) / (GAS_CONSTANT * (pressure_at_midpoint_log[i] - pressure_at_midpoint_log[i-1]));
		atm[i].p = (atm[i].T) * (atm[i].Nair * 1.0e6) * BOLTZMANN_CONSTANT / 1.0e2;/* 状態方程式 */
	}
	return atm;
}



ParamAtmosphere* Nair_to_atmosphere_old(
	int atm_Nheights,
	obsDateTime dt,
	Geocoordinate*& coord,
	PlanetParam earth,
	double* x,
	const double PRESSURE_AT_TOA//,
//	double* gm_e_arr
){
	constexpr double GM_E {3.986004356e14};/* 理科年表プレミアム 2025 */
//	constexpr double GM_E {3.7e14};/* MSISから逆算すると中高度ではだいたいこのくらいになったテキトウな値 *//* 重力が弱いと余計合わない */
	constexpr double BOLTZMANN_CONSTANT { 1.380649e-23 };

	ParamAtmosphere* atm = new ParamAtmosphere[atm_Nheights];/* will be returned*/
	for(int i=0; i<atm_Nheights; i++){
		atm[i].z = coord[i].altitude() * m2km;
		atm[i].Nair = x[i];/* これが主 */
		if(atm[i].z < 0){ atm[i].z = 0.0; }/* 誤差対策（たぶんいらない） */
	}
	atm[atm_Nheights-1].p = PRESSURE_AT_TOA;/* TOAではほぼ0気圧 */
	atm[atm_Nheights-1].T = (atm[atm_Nheights-1].p * 1.0e2) / (atm[atm_Nheights-1].Nair * 1.0e6)  / BOLTZMANN_CONSTANT;/* 状態方程式 */

	/* TODO これでいいの？ (uvspecの層の作り方) */
	for(int i=atm_Nheights-1; 0<i; i--){
		double radius = earth.radius() + (coord[i].altitude() + coord[i-1].altitude())/2.0;/* [m] */
		double graviational_acceleration = GM_E / radius / radius;
		double air_mass_density = ATM_MOL_W/*get_msis_average_molecular_weight( dt, coord[i-1] , coord[i])*/ * (atm[i-1].Nair + atm[i].Nair)/2  * 1.0e3;/* [kg m-3] */
		std::cerr << air_mass_density << std::endl;
		atm[i-1].p = atm[i].p + graviational_acceleration * air_mass_density / NA * (atm[i].z - atm[i-1].z)*1.0e3 * 1.0e-2;/* [hPa] *//* 静水圧平衡 *//* dz にマイナスを入れ込んだので問題ない */
		atm[i-1].T = (atm[i-1].p * 1.0e2)/ (atm[i-1].Nair * 1.0e6)  / BOLTZMANN_CONSTANT;/* 状態方程式 */
	}
	return atm;
}

