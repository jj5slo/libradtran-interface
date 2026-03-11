#ifndef __OBSERVED_H__
#define __OBSERVED_H__

#include<cmath>
#include<string>

#include"obsDateTime.h"

class Observed{/* ある方向の高度ごとのデータ */
private:
	double pLat = 0;
	double pLon = 0;
	double *pHeights;
	double *pData;
	int pNheights = 0;
public:
	Observed() : pHeights(nullptr), pData(nullptr) {}
	~Observed(){/* 使った配列を開放 */
		delete[] pHeights;
		delete[] pData;
	}
	Observed(const Observed &obj){
		pLat = obj.pLat;
		pLon = obj.pLon;
		pNheights = obj.pNheights;
		pHeights = new double [pNheights];
		pData    = new double [pNheights];
		for(int i=0; i<obj.pNheights; i++){
			pHeights[i] = obj.pHeights[i];
			pData[i]    = obj.pData[i];
		}
	}
	Observed& operator=(const Observed& obj){
		if(this == &obj){
			return *this;
		}
		delete[] pHeights;
		delete[] pData;
		
		pLat = obj.pLat;
		pLon = obj.pLon;
		pNheights = obj.pNheights;
		pHeights = new double [pNheights];
		pData    = new double [pNheights];
		for(int i=0; i<obj.pNheights; i++){
			pHeights[i] = obj.pHeights[i];
			pData[i]    = obj.pData[i];
		}

		return *this;
	}
	/* Observed.cpp */
	void set(double lat, double lon, int Nheights, double *heights, double *data);
	void set(double lat, double lon, int Nheights);
	double Latitude(void);
	double Longitude(void);
	double* Heights(void);/* [km] */
	double Data(double height);
	double* Data(void);
	double** Heights_Data(void);
	int Nheights(void);
	
	double maxHeight(void);


};

/* read_obs.cpp */
std::string obs_path(std::string path, obsDateTime dt);
Observed* read_obs(int *aNobs, std::string path_obs);/*, int Nline);*/
Observed read_obs(std::string path_obs, int obs_index);/*, int Nline);*/

#endif
