/* layered-atmosphere.h
 * 球状かつ層状に仮定した大気のプロファイルのクラス
 *
 */

/* 便利な定数 */
constexpr double PM32PCM3 { 1e-6 };/* /m^3 to /cm^3 . use like a[/m3]*2pcm3 [/cm3]*/
constexpr double PCM32PM3 { 1e6 };/* /cm^3 to /m^3 */

class Air{/* 大気諸量 */
private:
	/* 地理 */
	double lat = 0.0;
	double lon = 0.0;
	double alt = 0.0;

	/* 共通 */
	double p = 0.0;/* 圧力[hPa] */
	double T = 0.0;/* 温度[K] */
	double Nair = 0.0;/* 空気分子の数密度[cm-3] */
	double refractivem1 = 0.0;/* 屈折率 n-1 */

	/* 共通2 */
	double No2 = 0.0;
	double Nh2o = 0.0;
	double Nco2 = 0.0;
	double Nno2 = 0.0;

	/* libradtran */
	double No3 = 0.0;

	/* msis */
	
public:
	/* コンストラクタ */
	/* 座標情報 */
	Air(double alat, double alon, double aalt) : lat(alat), lon(alon), alt(aalt) {}

	/* アクセサ */
	/* 基本、与えられたパラメータだけ設定してあとは0で埋めたままの形になる。 */
	/* 再設定時0埋めをしていない（本当はしたほうがいい）ので注意。 */
	void setcoord( double alat, double alon, double aalt ){
		lat = alat;
		lon = alon;
		alt = aalt;
	}

	void set(ParamlibRadtran param){
		p = param.p;/* 圧力[hPa] */
		T = param.T;/* 温度[K] */
		Nair = param.Nair;/* 空気分子の数密度[cm-3] */
	
		No3 = param.No3;
		No2 = param.No2;
		Nh2o = param.Nh2o;
		Nco2 = param.Nco2;
		Nno2 = param.Nno2;	
	}	
	 
	void standard(double alat, double alon, double aalt){
		// TODO MSISかなにかから標準大気を持ってくる	
	}

	/* 出力 */
	double* returnlibRadtran(void);/* libRadtran入力用 */
	double* returnrayleigh(void);/* rayleighscatterのみの計算入力用 */
	
	/* 計算 */
	// void 
}

class Atmosphere{/* Airを層状(など)に束ねる *//* 緯度経度で変えるとかするならこれの配列を作ってください */
private:
	int p_N;/* 全N層 */
	Air *p_air;
	double p_totalheight;
	double p_dr;
public:
	/* コンストラクタ */
	Atmosphere(Geocoordinate coord, double totalheight, int n ){
		set(coord, totalheight, n);
	}
	void set(Geocoordinate coord, double totalheight, int n );

	/* アクセサ */
	void setair( ParamlibRadtran* params );

	double** returnlibRadtran(void);



	deleteall(){
		// TODO デストラクタ？
	}

}
