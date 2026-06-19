#include<iostream>
#include<cmath>

#include "Vector3d.h"

#include "readwrite.h"
#include "FixedParams.h"
#include "coordinate.h"

double lat0s[88] = { 38.0,  39.0,  40.0,  41.0,  42.0,  43.0,  44.0,  45.0,  46.0,  47.0,  48.0,  49.0,  50.0,  51.0,  52.0,  53.0,  54.0,  55.0,  56.0,  57.0,  58.0,  59.0,  60.0,  61.0,  62.0,  63.0,  64.0,  65.0,  66.0,  67.0,  68.0,  69.0,  70.0,  71.0,  72.0,  73.0,  74.0,  75.0,  76.0,  77.0,  78.0,  79.0,  80.0,  81.0,  81.0,  80.0,  79.0,  78.0,  77.0,  76.0,  75.0,  74.0,  73.0,  72.0,  71.0,  70.0,  69.0,  68.0,  67.0,  66.0,  65.0,  64.0,  63.0,  62.0,  61.0,  60.0,  59.0,  58.0,  57.0,  56.0,  55.0,  54.0,  53.0,  52.0,  51.0,  50.0,  49.0,  48.0,  47.0,  46.0,  45.0,  44.0,  43.0,  42.0,  41.0,  40.0,  39.0,  38.0};
double lon0s[88] = { 61.8,  62.0,  62.2,  62.3,  62.5,  62.7,  62.9,  63.1,  63.4,  63.6,  63.8,  64.1,  64.4,  64.7,  65.0,  65.3,  65.7,  66.1,  66.5,  66.9,  67.4,  67.9,  68.4,  69.0,  69.6,  70.3,  71.0,  71.8,  72.7,  73.6,  74.6,  75.8,  77.1,  78.5,  80.2,  82.0,  84.2,  86.7,  89.7,  93.3,  97.8, 103.7, 112.3, 128.8, 152.6, 169.1, 177.7, 183.6, 188.1, 191.7, 194.7, 197.2, 199.4, 201.2, 202.9, 204.3, 205.6, 206.8, 207.8, 208.7, 209.6, 210.4, 211.1, 211.8, 212.4, 213.0, 213.5, 214.0, 214.5, 214.9, 215.3, 215.7, 216.1, 216.4, 216.7, 217.0, 217.3, 217.6, 217.8, 218.0, 218.3, 218.5, 218.7, 218.9, 219.1, 219.2, 219.4, 219.6};


/*
 * Geocoordinate で観測点の
 *
 *
 *
 *
 *
 */


int main(void){
	PlanetParam    earth(6370.e3);
	SatelliteParam himawari( 35790.e3 + earth.radius(), 0.0, 140.7 );
	double step = 100e3;/* [m] */
	/* lat, lon, height は tangential point のもの */
	/* x y lat lon lat(ground) lon(ground) height alpha */
	double** ret = new double* [8];
	int Ndata = ( std::ceil(earth.radius() / step) + 1 ) * ( 2*std::ceil(earth.radius() / step) + 1);
	for(int j=0; j<8; ++j){
		ret[j] = new double[Ndata];
	}

	int count = 0;
	for(int i=0; i<=std::ceil(earth.radius() / step); i++){
		for(int j=-1*std::ceil(earth.radius() / step); j<=std::ceil(earth.radius() / step); j++){
			AndoLab::Vector3d<double> r(0.0, step*j, step*i);
			Geocoordinate coord(earth, himawari, r);
			double alpha = coord.alpha()*AndoLab::RAD2DEG;
			/* tangential point の altitude は その点と衛星を結ぶ視線におろした原点からの垂線の長さ（から地球半径を引いたもの）。*/
			AndoLab::Vector3d<double> u = AndoLab::Vector3d(himawari.radius(), 0.0, 0.0) - coord.r();
			AndoLab::Vector3d<double> tangential_point_vec = r + (-1*(r%u) / (u%u))*u;
//			double altitude = std::sqrt(tangential_point_vec%tangential_point_vec) - earth.radius();
			Geocoordinate tangential_point(earth, himawari, tangential_point_vec);
			
			LookingDirection ld0(alpha, 0.0);
			Geocoordinate on_ground = ld0.tangential_point(earth, himawari);

			ret[0][count] = r.y() / 1.0e3;// x(y)
			ret[1][count] = r.z() / 1.0e3;// y(z)
			ret[2][count] = tangential_point.latitude();
			ret[3][count] = tangential_point.longitude();
			ret[4][count] = on_ground.latitude();
			ret[5][count] = on_ground.longitude();
			ret[6][count] = tangential_point.altitude_in_km();
			ret[7][count] = alpha;
			
			if(++count > Ndata){
				std::cerr << "Error" << std::endl;
				for(int j=0; j<8; ++j){
					delete[] ret[j];
				}
				delete[] ret;
				return 1;
			}
		}
	}

	readwrite::save_data("grid.dat", "# x[km] y[km] lat lon lat(ground) lon(ground) height[km] alpha", Ndata, 8, ret);

	for(int j=0; j<7; ++j){
		delete[] ret[j];
	}
	delete[] ret;



	step = 0.200;/* [deg] */
	/* lat, lon, height は tangential point のもの */
	/* x y lat lon lat(ground) lon(ground) height alpha theta phi */
	ret = new double* [10];
	Ndata = ( std::ceil(std::atan((earth.radius() + 100e3)/himawari.radius())*180/M_PI / step ) + 1 ) * ( 2*std::ceil(std::atan((earth.radius() + 100e3)/himawari.radius())*180/M_PI / step) + 1);
	for(int j=0; j<10; ++j){
		ret[j] = new double[Ndata];
	}

	count = 0;
	for(int i=0; i<=std::ceil(std::atan((earth.radius() + 100e3)/himawari.radius())*180/M_PI / step); i++){
		for(int j=-1*std::ceil(std::atan((earth.radius() + 100e3)/himawari.radius())*180/M_PI / step); j<=std::ceil(std::atan((earth.radius() + 100e3)/himawari.radius())*180/M_PI / step); j++){
			AndoLab::Vector3d<double> r(0.0, himawari.radius()*std::tan(step*j*M_PI/180.0), himawari.radius()*std::tan(step*i*M_PI/180.0));
			Geocoordinate coord(earth, himawari, r);
			double alpha = coord.alpha()*AndoLab::RAD2DEG;
			/* tangential point の altitude は その点と衛星を結ぶ視線におろした原点からの垂線の長さ（から地球半径を引いたもの）。*/
			AndoLab::Vector3d<double> u = AndoLab::Vector3d(himawari.radius(), 0.0, 0.0) - coord.r();
			AndoLab::Vector3d<double> tangential_point_vec = r + (-1*(r%u) / (u%u))*u;
//			double altitude = std::sqrt(tangential_point_vec%tangential_point_vec) - earth.radius();
			Geocoordinate tangential_point(earth, himawari, tangential_point_vec);
			
			LookingDirection ld0(alpha, 0.0);
			Geocoordinate on_ground = ld0.tangential_point(earth, himawari);

			ret[0][count] = r.y() / 1.0e3;// x(y)
			ret[1][count] = r.z() / 1.0e3;// y(z)
			ret[2][count] = tangential_point.latitude();
			ret[3][count] = tangential_point.longitude();
			ret[4][count] = on_ground.latitude();
			ret[5][count] = on_ground.longitude();
			ret[6][count] = tangential_point.altitude_in_km();
			ret[7][count] = alpha;
			ret[8][count] = step*i;
			ret[9][count] = step*j;
			
			if(++count > Ndata){
				std::cerr << "Error" << std::endl;
				for(int j=0; j<8; ++j){
					delete[] ret[j];
				}
				delete[] ret;
				return 1;
			}
		}
	}

	readwrite::save_data("grid_angle0.2deg.dat", "# x[km] y[km] lat lon lat(ground) lon(ground) height[km] alpha theta phi", Ndata, 10, ret);

	for(int j=0; j<7; ++j){
		delete[] ret[j];
	}
	delete[] ret;


/* lat(ground) lon(ground) height alpha theta phi */
	ret = new double* [6];
	for(int i=0; i<6; ++i){
		ret[i] = new double[88*101];
	}

	int index = 0; // retの1次元インデックスを管理

	

	for(int i=0; i<88; ++i){
		double lat = lat0s[i];
		double lon = lon0s[i];
		
		Geocoordinate on_ground(earth, himawari, lat, lon, 0.0);
		double ground_alpha = on_ground.alpha() * AndoLab::RAD2DEG; // 変数名を変更

		for (int h=0; h<101; ++h){
			double current_height = (double)h;
			
			LookingDirection ld(ground_alpha, current_height);
			Geocoordinate tp = ld.tangential_point(earth, himawari);
			
			double tp_alpha = tp.alpha();
			double theta    = std::atan( tp.r().z() / (himawari.radius() - tp.r().x()) ) * AndoLab::RAD2DEG; /* 南北方向 */
			double phi      = std::atan( tp.r().y() / (himawari.radius() - tp.r().x()) ) * AndoLab::RAD2DEG; /* 東西方向 */

			// 2次元配列を介さず、直接 ret に格納する
			ret[0][index] = lat;
			ret[1][index] = lon;
			ret[2][index] = current_height;
			ret[3][index] = tp_alpha;
			ret[4][index] = theta;
			ret[5][index] = phi;

			index++; // 次のデータ点へ
		}
	}

	readwrite::save_data("table_obs_angle.dat", "# lat(ground) lon(ground) height[km] alpha theta phi", 88*101, 6, ret, 12);
	
	for(int j=0; j<6; ++j){
		delete[] ret[j];
	}
	delete[] ret;

	return 0;


//	/* lat(ground) lon(ground) height alpha theta phi */
//	double** heights = new double* [88];
//	double** alphas = new double* [88];
//	double** thetas = new double* [88];
//	double** phis = new double* [88];
//	for(int i=0; i<88; ++i){
//		heights[i] = new double [101];
//		alphas[i] = new double [101];
//		thetas[i] = new double [101];
//		phis[i] = new double [101];
//	}
//	for(int i=0; i<88; ++i){
//		for(int j=0; j<88; ++j){
//			double lat = lat0s[i];
//			double lon = lon0s[i];
//			Geocoordinate on_ground(earth, himawari, lat, lon, 0.0);
//			double alpha = on_ground.alpha()*AndoLab::RAD2DEG;
//			for (int height=0; height<101; ++height){
//				heights[i][height] = (double)height;
//				LookingDirection ld(alpha, heights[i][height]);
//				Geocoordinate tp = ld.tangential_point();
//				double alpha = tp.alpha();
//				double theta = std::atan( tp.r().z() / himawari.radius() );/* 南北方向 */
//				double phi   = std::atan( tp.r().y() / himawari.radius() );/* 東西方向 */
//				alphas[i][height] = alpha;
//				thetas[i][height] = theta;
//				phis[i][height] = phi;
//		}
//	}
//	double** lats_repeat = new double* [88];
//	double** lons_repeat = new double* [88];
//	for(int i=0; i<88; ++i){
//		lats_repeat[i] = new double [101];
//		lons_repeat[i] = new double [101];
//		for (int j=0; j<101; ++j){
//			lats_repeat[i][j] = lat0s[i];
//			lons_repeat[i][j] = lon0s[i];
//		}
//	}
//
//	ret = new double* [6];
//	for(int i=0; i<6; ++i){
//		ret[i] = new double[88*101];
//	}
//	for(int j=0; j<88*101; ++j){
//		ret[0][j] = lats_repeat[j/101][j%101];
//		ret[1][j] = lons_repeat[j/101][j%101];
//		ret[2][j] = alphas[j/101][j%101];
//		ret[3][j] = thetas[j/101][j%101];
//		ret[4][j] = phis[j/101][j%101];
//	}
//
//	readwrite::save_data("table_obs_angle.dat", "# lat(ground) lon(ground) height[km] alpha theta phi", 88*101, 6, ret);
	
	return 0;
}
