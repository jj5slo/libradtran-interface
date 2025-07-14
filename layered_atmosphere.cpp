/* 
 * layered-atmosphere.cpp
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
/* height [m] を n層に分割（等分） */
	p_N = n;
	p_air = new Air [n];
	double p_totalheight = totalheight;
	for(int i = 0; i<N; i++){
		air[i].setcoord( coord.latitude(), coord.longitude, range/N );/* 高度のみ等分割して初期化 */
	}
}


void Atmosphere::setair( ParamlibRadtran* params ){
	for(int i=0; i<N; i++){
		air[i].set(params[i]);
}

void Air::returnlibRadtran(){
	double* values;
}

void Atmosphere::returnlibRadtran(){
	double** matrix;


	return matrix;
}
