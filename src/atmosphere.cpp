/* 
 * atmosphere.cpp
 * 関数
 *
 *
 *
 *
 *
 *
 *
 *
 *
 */

#include<iostream>

#include"layered-atmosphere.h"


void Atmosphere::set(Geocoordinate coord, double totalheight, int n){
/* Airをn層用意する */
	p_N = n;
	p_air = new Air [n];
	double p_totalheight = totalheight;
	for(int i = 0; i<N; i++){
		air[i].setcoord( coord.latitude(), coord.longitude, range/N );/* 高度のみ等分割して初期化 */
	}
}


void Atmosphere::setair( int i, Geocoordinate coord,  ){
		air[i].set(params[i]);
}
void Atmosphere::setair(   ){
		air[i].set(params[i]);
}

ParamAtmosphere Atmosphere::libRadtran(std::string path){/* libRadtranの大気ファイルを作る */
	std::ofstream ofs(path);/* ここに保存 */

	if(!ofs){
		std::cerr << "Failed to create file '" << path << "'" << std::endl;
		return 1;
	}



}

void Air::returnlibRadtran(){
	double* values;
}

void Atmosphere::returnlibRadtran(){
	double** matrix;


	return matrix;
}
