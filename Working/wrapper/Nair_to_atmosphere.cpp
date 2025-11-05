/*
 * Nair_to_atmosphere
 * 静水圧平衡で求める。
 *
 */


#include "wrapper.h"

ParamAtmosphere* Nair_to_atmosphere(
	int atm_Nheights,
	obsDateTime dt,
	Geocoordinate*& coord,
	PlanetParam earth,
	std::vector<double> x,
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

//		double GM_E = gm_e_arr[i-1];

		double graviational_acceleration = GM_E / radius / radius;
		double air_mass_density = get_msis_total_mass_density( dt, coord[i-1] , coord[i]) * 1.0e3;/* [kg m-3] */
		std::cerr << air_mass_density << std::endl;
		atm[i-1].p = atm[i].p + graviational_acceleration * air_mass_density * (atm[i].z - atm[i-1].z)*1.0e3 * 1.0e-2;/* [hPa] *//* 静水圧平衡 *//* dz にマイナスを入れ込んだので問題ない */
		atm[i-1].T = (atm[i-1].p * 1.0e2)/ (atm[i-1].Nair * 1.0e6)  / BOLTZMANN_CONSTANT;/* 状態方程式 */
	}
	return atm;
}

