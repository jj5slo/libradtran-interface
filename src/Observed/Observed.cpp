#include"Observed.h"

void Observed::set(double lat, double lon, int Nheights, double *heights, double *data){
	pLat = lat;
	pLon = lon;
	pNheights = Nheights;
	pHeights = new double[Nheights];
	for(int i=0; i<Nheights; i++){ pHeights[i] = heights[i]; }
	pData = new double[Nheights];
	for(int i=0; i<Nheights; i++){ pData[i] = data[i]; }
}

double Observed::Latitude(void){
	return pLat;
}
double Observed::Longitude(void){
	return pLon;
}
double* Observed::Heights(void){
	return pHeights;
}
int Observed::Nheights(void){
	return pNheights;
}
double Observed::Data(double height){/* いらない */
	for(int i=0; i<pNheights; i++){
		if(std::abs(pHeights[i] - height) < 0.1){/* TODO */
			return pData[i];
		}
	}
	return 0.0;/* no match */
}
double* Observed::Data(void){
	return pData;
}
double** Observed::Heights_Data(void){
	double** hd = new double*[2];
	hd[0] = pHeights;
	hd[1] = pData;
	return hd;
}


double Observed::maxHeight(){
	double maxh = pHeights[0];
	for(int i=1; i<pNheights; i++){
		if(maxh < pHeights[i]){ maxh = pHeights[i]; }
	}
	return maxh;
}

