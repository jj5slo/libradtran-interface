/*
 * get_msis.cpp
 *
 */
#include"get_msis.h"



ParamAtmosphere* get_msis(obsDateTime dt, Geocoordinate* coord, int N){
	/* dt, coord(lat, lon, alt) からMSISのデータ（空気分子数密度にするだけでいいか？）にする*/

	/* NRL-MSISE00の入出力用 */
	using Ap_array = struct ap_array;
	using NRLMSISE_Input = struct nrlmsise_input;
	using NRLMSISE_Flag = struct nrlmsise_flags;
	using NRLMSISE_Output = struct nrlmsise_output;

	constexpr double BOLTZMANN_CONSTANT { 1.380649e-23 };
	
  Ap_array Xp;
  NRLMSISE_Input Input;
  NRLMSISE_Flag Flag;
  NRLMSISE_Output Output;
	
	/* TODO 各分子数密度から大気分子数密度 */

  for(int i = 0; i < 7; i++){
    Xp.a[i] = 100.0;/* 磁気指数 標準的な値 */
  }
  Flag.switches[0] = 0; /* kg, m を使用しないで g, cm を使用 */
  for(int i = 1; i < 24; i++){
    Flag.switches[i] = 1;
  }

	/* -- */

  Input.year = dt.Year(); /* 年は機能していないと思う */
  Input.doy = dt.DOY(); /* Day of Year; 1月1日を1、2月1日を32、...、12月31日を365とする(除く閏年) */
  Input.sec = dt.DaySecond(); /* 0時0分を 0、23時59分を86400-1 */
  Input.lst = Input.sec / 3600 + Input.g_long / 15; /* [hours] local apparent solar time, lst = sec/3600 + g_long/15 にする */
  Input.f107A = 150.; /* F10.7フラックスの81日平均。標準的な値 */
  Input.f107 = 150.; /* 当日のF10.7フラックス。標準的な値 */
  Input.ap = 4.0; /* 磁気指数。標準的な値 */
  Input.ap_a = &Xp; /* 同上 */

	
	/* -- */


//	Atmosphere atm_msis(coord[0], N);
	ParamAtmosphere *atm_msis = new ParamAtmosphere[N];/* これを返す。 */

	for(int i=0; i<N; i++){
	
	  Input.g_lat = coord[i].latitude(); /* [deg] 緯度 */
	  Input.g_long = coord[i].longitude();/* [deg] 経度、西経は負 */
	  Input.alt = coord[i].altitude() * m2km; /* [km] 高度 */
	
	  gtd7(&Input, &Flag, &Output);/* Neutral Atmosphere Empircial Model from the surface to lower exosphere. */
	
		/* TODO */
	

//	  for(int i = 0; i < 9; i++){
//	    std::cout << i << " " << Output.d[i] << std::endl;
//	  }
//	  for(int i = 0; i < 2; i++){
//	    std::cout << i << " " << Output.t[i] << std::endl;
//	  }
	
	//  std::ofstream ofs("msise.dat");
		
    /*
     * 出力 (指定された高度)
     *      d[0] - He 数密度 [cm^{-3}]
     *      d[1] - O 数密度 [cm^{-3}]
     *      d[2] - N2 数密度 [cm^{-3}]
     *      d[3] - O2 数密度 [cm^{-3}]
     *      d[4] - Ar 数密度 [cm^{-3}]                       
     *      d[5] - 全質量密度 [g/cm^3] [includes d[8] in td7d]
     *      d[6] - H 数密度 [cm^{-3}]
     *      d[7] - N 数密度 [cm^{-3}]
     *      d[8] - Anomalous oxygen 数密度 [cm^{-3}]
     *      t[0] - [K] EXOSPHERIC TEMPERATURE (外気圏温度)
     *      t[1] - [K] 温度
     */

    double n = 0.0;
    for(int j = 0; j < 5; j++){
      n += Output.d[j];
    }
    n += Output.d[6] + Output.d[7]; /* [cm^{-3}] */

    double p = (n*1e6) * BOLTZMANN_CONSTANT * Output.t[1] * 1e-2;
		

		atm_msis[i].z = coord[i].altitude() * m2km; /* [km] 高度 */
		atm_msis[i].Nair = n;
		atm_msis[i].p = p;
		atm_msis[i].T = Output.t[1];	
		
		if(atm_msis[i].z < 0){ atm_msis[i].z = 0.0; }

  }
//	  ofs.close();
	return atm_msis;
}

double get_msis_total_mass_density(obsDateTime dt, Geocoordinate coord){
	/* NRL-MSISE00の入出力用 */
	using Ap_array = struct ap_array;
	using NRLMSISE_Input = struct nrlmsise_input;
	using NRLMSISE_Flag = struct nrlmsise_flags;
	using NRLMSISE_Output = struct nrlmsise_output;
	
  Ap_array Xp;
  NRLMSISE_Input Input;
  NRLMSISE_Flag Flag;
  NRLMSISE_Output Output;
	
  for(int i = 0; i < 7; i++){
    Xp.a[i] = 100.0;/* 磁気指数 標準的な値 */
  }
  Flag.switches[0] = 0; /* kg, m を使用しないで g, cm を使用 */
  for(int i = 1; i < 24; i++){
    Flag.switches[i] = 1;
  }

	/* -- */
  Input.year = dt.Year(); /* 年は機能していないと思う */
  Input.doy = dt.DOY(); /* Day of Year; 1月1日を1、2月1日を32、...、12月31日を365とする(除く閏年) */
  Input.sec = dt.DaySecond(); /* 0時0分を 0、23時59分を86400-1 */
  Input.lst = Input.sec / 3600 + Input.g_long / 15; /* [hours] local apparent solar time, lst = sec/3600 + g_long/15 にする */
  Input.f107A = 150.; /* F10.7フラックスの81日平均。標準的な値 */
  Input.f107 = 150.; /* 当日のF10.7フラックス。標準的な値 */
  Input.ap = 4.0; /* 磁気指数。標準的な値 */
  Input.ap_a = &Xp; /* 同上 */	
	/* -- */


	Input.g_lat = coord.latitude(); /* [deg] 緯度 */
	Input.g_long = coord.longitude();/* [deg] 経度、西経は負 */
	Input.alt = coord.altitude() * m2km; /* [km] 高度 */
	
	gtd7(&Input, &Flag, &Output);/* Neutral Atmosphere Empircial Model from the surface to lower exosphere. */	
    /*
     * 出力 (指定された高度)
     *      d[0] - He 数密度 [cm^{-3}]
     *      d[1] - O 数密度 [cm^{-3}]
     *      d[2] - N2 数密度 [cm^{-3}]
     *      d[3] - O2 数密度 [cm^{-3}]
     *      d[4] - Ar 数密度 [cm^{-3}]                       
     *      d[5] - 全質量密度 [g/cm^3] [includes d[8] in td7d]
     *      d[6] - H 数密度 [cm^{-3}]
     *      d[7] - N 数密度 [cm^{-3}]
     *      d[8] - Anomalous oxygen 数密度 [cm^{-3}]
     *      t[0] - [K] EXOSPHERIC TEMPERATURE (外気圏温度)
     *      t[1] - [K] 温度
     */
	return Output.d[5];
}


double get_msis_total_mass_density(/* 全質量密度を返す */
	obsDateTime dt, Geocoordinate coord1, Geocoordinate coord2
){
	Geocoordinate coord ( coord1.planet(), coord1.satellite(), ( coord1.r() + coord2.r() ) / 2.0 );
	return get_msis_total_mass_density(dt, coord);
}

double get_msis_average_molecular_weight(obsDateTime dt, Geocoordinate coord){
	/* NRL-MSISE00の入出力用 */
	using Ap_array = struct ap_array;
	using NRLMSISE_Input = struct nrlmsise_input;
	using NRLMSISE_Flag = struct nrlmsise_flags;
	using NRLMSISE_Output = struct nrlmsise_output;
	
  Ap_array Xp;
  NRLMSISE_Input Input;
  NRLMSISE_Flag Flag;
  NRLMSISE_Output Output;
	
  for(int i = 0; i < 7; i++){
    Xp.a[i] = 100.0;/* 磁気指数 標準的な値 */
  }
  Flag.switches[0] = 0; /* kg, m を使用しないで g, cm を使用 */
  for(int i = 1; i < 24; i++){
    Flag.switches[i] = 1;
  }

	/* -- */
  Input.year = dt.Year(); /* 年は機能していないと思う */
  Input.doy = dt.DOY(); /* Day of Year; 1月1日を1、2月1日を32、...、12月31日を365とする(除く閏年) */
  Input.sec = dt.DaySecond(); /* 0時0分を 0、23時59分を86400-1 */
  Input.lst = Input.sec / 3600 + Input.g_long / 15; /* [hours] local apparent solar time, lst = sec/3600 + g_long/15 にする */
  Input.f107A = 150.; /* F10.7フラックスの81日平均。標準的な値 */
  Input.f107 = 150.; /* 当日のF10.7フラックス。標準的な値 */
  Input.ap = 4.0; /* 磁気指数。標準的な値 */
  Input.ap_a = &Xp; /* 同上 */	
	/* -- */


	Input.g_lat = coord.latitude(); /* [deg] 緯度 */
	Input.g_long = coord.longitude();/* [deg] 経度、西経は負 */
	Input.alt = coord.altitude() * m2km; /* [km] 高度 */
	
	gtd7(&Input, &Flag, &Output);/* Neutral Atmosphere Empircial Model from the surface to lower exosphere. */	
    /*
     * 出力 (指定された高度)
     *      d[0] - He 数密度 [cm^{-3}]
     *      d[1] - O 数密度 [cm^{-3}]
     *      d[2] - N2 数密度 [cm^{-3}]
     *      d[3] - O2 数密度 [cm^{-3}]
     *      d[4] - Ar 数密度 [cm^{-3}]                       
     *      d[5] - 全質量密度 [g/cm^3] [includes d[8] in td7d]
     *      d[6] - H 数密度 [cm^{-3}]
     *      d[7] - N 数密度 [cm^{-3}]
     *      d[8] - Anomalous oxygen 数密度 [cm^{-3}]
     *      t[0] - [K] EXOSPHERIC TEMPERATURE (外気圏温度)
     *      t[1] - [K] 温度
     */
    double n = 0.0;
    for(int j = 0; j < 5; j++){
      n += Output.d[j];
    }
    n += Output.d[6] + Output.d[7]; /* [cm^{-3}] */
	return Output.d[5] / n;
}


double get_msis_average_molecular_weight(/* 平均分子量を返す */
	obsDateTime dt, Geocoordinate coord1, Geocoordinate coord2
){
	Geocoordinate coord ( coord1.planet(), coord1.satellite(), ( coord1.r() + coord2.r() ) / 2.0 );
	return get_msis_average_molecular_weight(dt, coord);
}
