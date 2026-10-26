
#include "atmNair_to_temperature.h"

/*

  TODO iTOP以下だけ温度を求める。
	     それ以上は元のファイルから埋める
 */



ParamAtmosphere* call_temperatures(PlanetParam earth, ParamAtmosphere*& rawAtm, const int Nheights, const int iTOP, const double COEF){
	
		double* heights = new double [Nheights];
		double* x       = new double[Nheights];

		for(int i=0; i<Nheights; i++){
			heights[i] = rawAtm[i].z;
			x[i]       = rawAtm[i].Nair;
		}
		
//		auto earth = PlanetParam( 6370.e3 );
	
		double pressure_at_top;
		pressure_at_top = COEF * std::pow(10, (std::log10(rawAtm[iTOP+1].p) + std::log10(rawAtm[iTOP].p))/2.0);

		ParamAtmosphere* newatm = Nair_to_atmosphere_new(Nheights, earth, heights, x, iTOP, pressure_at_top);
		
		return newatm;
}


void Nair_to_Temperature(
	int         atm_Nheights,
	ParamAtmosphere*& atm,/* Nair以外はMSIS */
	int         iTOP,/* 固定する点!! */
	PlanetParam earth
){
	constexpr double GM_E {3.986004356e14};/* 理科年表プレミアム 2025 */
	constexpr double KB { 1.380649e-23 };
	constexpr double NA {6.02214076e23};
	
	const double m = ATM_MOL_W * 1.0e-3 / NA;
	const double R = earth.radius();/* [m] */
	
	for(int i=iTOP-1; i>=0; --i){
		const double z1 = atm[i].z * 1.0e3;
		const double z0 = atm[i+1].z * 1.0e3;
		const double dz = z0 - z1;
		const double k = std::log(atm[i].Nair / atm[i+1].Nair) / dz;/* eを底とする指数係数 [np m^-1] */
		
		/* 台形積分 */
		constexpr int NSUB = 100;
		const double dx = dz / NSUB;
		double integral = 0.0;
		for (int j=0; j <= NSUB; ++j){
			const double x = dx*j;
			const double radius = R + z1 + x;
			const double f = std::exp(-k*x) / (radius*radius);
			const double weight = (j == 0 || j == NSUB) ? 0.5 : 1.0;/* 端は重み0.5 */
			integral += weight * f;
		}
		integral *= dx;

		/* T_i = T_{i+1}\exp (-k_i \Delta z_i) + \frac{mGM_E}{k_B} \int_0^{\Delta z_i} \frac{\exp(-k_i x)}{(R_E + z_i + x)^2} dx */
		atm[i].T = atm[i+1].T * std::exp(-k*dz) + (m*GM_E / KB)*integral;

		atm[i].set_p_from_Nair_T();
	}
}

	

ParamAtmosphere* Nair_to_atmosphere_new(
	int atm_Nheights,
	PlanetParam earth,
	double* heights,
	double* x,
	int     iTOP,
	const double PRESSURE_AT_TOP_MID /* iTOP + 1/2 */
){
	constexpr double GM_E {3.986004356e14};/* 理科年表プレミアム 2025 */
	constexpr double BOLTZMANN_CONSTANT { 1.380649e-23 };
	constexpr double NA {6.02214076e23};
	constexpr double GAS_CONSTANT = NA*BOLTZMANN_CONSTANT;

	ParamAtmosphere* atm = new ParamAtmosphere[atm_Nheights];/* will be returned*/
	for(int i=0; i<atm_Nheights; i++){
		atm[i].z = heights[i];
		atm[i].Nair = x[i];/* これが主 */
		if(atm[i].z < 0){ atm[i].z = 0.0; }/* 誤差対策（たぶんいらない） */
	}
//	atm[atm_Nheights-1].p = PRESSURE_AT_TOA;/* TOAではほぼ0気圧 */

	double* pressure_at_midpoint = new double[atm_Nheights - 1];
	double* pressure_at_midpoint_log = new double[atm_Nheights - 1];
	pressure_at_midpoint[iTOP] = PRESSURE_AT_TOP_MID;
	pressure_at_midpoint_log[iTOP] = std::log(PRESSURE_AT_TOP_MID);
	for(int i=iTOP-1; 0<=i; i--){
		double radius = earth.radius() + heights[i]/m2km;/* [m] */
		double graviational_acceleration = GM_E / radius / radius;
		double Dz = (atm[i+2].z - atm[i].z) / 2.0 / m2km;/* [m] */
		
		pressure_at_midpoint[i] 
			= pressure_at_midpoint[i+1] 
				+ ATM_MOL_W*1.0e-3 * (atm[i+1].Nair*1.0e6)  / NA * graviational_acceleration * Dz * 1.0e-2;
		pressure_at_midpoint_log[i] = std::log(pressure_at_midpoint[i]);
	}

	for(int i=1; i<=iTOP; i++){
		double radius = earth.radius() + heights[i]/m2km;/* [m] */
		double graviational_acceleration = GM_E / radius / radius;
		double Dz = (atm[i].z - atm[i-1].z) / m2km;
		atm[i].T = - (ATM_MOL_W*1.0e-3 * graviational_acceleration * Dz) / (GAS_CONSTANT * (pressure_at_midpoint_log[i] - pressure_at_midpoint_log[i-1]));
		atm[i].p = (atm[i].T) * (atm[i].Nair * 1.0e6) * BOLTZMANN_CONSTANT * 1.0e-2;/* 状態方程式 */
	}
	return atm;
}



ParamAtmosphere* Nair_to_atmosphere_old(
	int atm_Nheights,
	PlanetParam earth,
	double* heights,
	double* x,
	int     iTOP,
	const double PRESSURE_AT_TOP/* iTOP + 1 *///,
//	double* gm_e_arr
){
	constexpr double GM_E {3.986004356e14};/* 理科年表プレミアム 2025 */
//	constexpr double GM_E {3.7e14};/* MSISから逆算すると中高度ではだいたいこのくらいになったテキトウな値 *//* 重力が弱いと余計合わない */
	constexpr double BOLTZMANN_CONSTANT { 1.380649e-23 };

	ParamAtmosphere* atm = new ParamAtmosphere[atm_Nheights];/* will be returned*/
	for(int i=0; i<atm_Nheights; i++){
		atm[i].z = heights[i];
		atm[i].Nair = x[i];/* これが主 */
		if(atm[i].z < 0){ atm[i].z = 0.0; }/* 誤差対策（たぶんいらない） */
	}
	atm[atm_Nheights-1].p = PRESSURE_AT_TOP;/* TOAではほぼ0気圧 */
	atm[atm_Nheights-1].T = (atm[atm_Nheights-1].p * 1.0e2) / (atm[atm_Nheights-1].Nair * 1.0e6)  / BOLTZMANN_CONSTANT;/* 状態方程式 */

	/* TODO これでいいの？ (uvspecの層の作り方) */
	for(int i=atm_Nheights-1; 0<i; i--){
		double radius = earth.radius() + (heights[i]/m2km + heights[i-1]/m2km)/2.0;/* [m] */
		double graviational_acceleration = GM_E / radius / radius;
		double air_mass_density = ATM_MOL_W*1.0e-3/*get_msis_average_molecular_weight( dt, coord[i-1] , coord[i])*/ * std::pow(10, ((std::log10(atm[i-1].Nair) + std::log10(atm[i].Nair))/2))  * 1.0e6;/* [kg m-3] */
//		std::cerr << air_mass_density << std::endl;
		atm[i-1].p = atm[i].p + graviational_acceleration * air_mass_density / NA * (atm[i].z - atm[i-1].z)*1.0e3 * 1.0e-2;/* [hPa] *//* 静水圧平衡 *//* dz にマイナスを入れ込んだので問題ない */
		atm[i-1].T = (atm[i-1].p * 1.0e2)/ (atm[i-1].Nair * 1.0e6)  / BOLTZMANN_CONSTANT;/* 状態方程式 */
	}
	return atm;
}

