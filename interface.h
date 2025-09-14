#ifndef __INTERFACE_H__
#define __INTERFACE_H__

class ParamStdin{/* stdin に渡すパラメタをまとめたクラス（後で拡張しやすいように） */
private:
public:
//	enum solver {mystic_spherical};
	double wavelength {400.0};
	double sza {0.0};
	double phi0 {0.0};
	double umu {0.0};
	double phi {0.0};
	double albedo{0.0};

	int mc_photons {1000000};

	std::string solver = "mystic";
	std::string additional = "";

};

class ParamAtmosphere{/* libRadtranの大気プロファイルに設定できる諸量 */
private:
	int p_NoPs = 9; /* パラメタの数 */
public:
	double z;
	double p;/* 圧力[hPa] */
	double T;/* 温度[K] */
	double Nair;/* 空気分子の数密度[cm-3] */
	double No3 ;
	double No2 ;
	double Nh2o;
	double Nco2;
	double Nno2;
	
	ParamAtmosphere(void) : z(0.0), p(0.0), T(0.0), Nair(0.0), No3(0.0), No2(0.0), Nh2o(0.0), Nco2(0.0), Nno2(0.0) {}

	/* radtran-interface-atmosphere.cpp */
	double* returnvector(void);
	int NoPs(void){
		return p_NoPs;
	}
};
/* TODO 標準的な値を出す関数を作る（ファイル参照でもなんでもいいので） */


/* interface_stdin.cpp */
int save_stdin(std::string path_stdin, ParamStdin param);
	/* 0:無問題, 1:ERROR */


/* interface-atmosphere.cpp */
void saveParams(std::string filename, ParamAtmosphere *params, int Nlines, int Ndecimal = 6);

#endif /* __INTERFACE_H__ */

