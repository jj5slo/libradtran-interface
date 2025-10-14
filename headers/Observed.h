#ifndef __OBSERVED_H__
#define __OBSERVED_H__

#include<cmath>
#include<string>

#include"obsDateTime.h"

class Observed{/* ある方向の高度ごとのデータ */
private:
	double pLat;
	double pLon;
	double *pHeights;
	double *pData;
	int pNheights;
public:
	/* Observed.cpp */
	void set(double lat, double lon, int Nheights, double *heights, double *data);
	double Latitude(void);
	double Longitude(void);
	double* Heights(void);/* [km] */
	double Data(double height);
	int Nheights(void);
	
	double maxHeight(void);

	~Observed(){/* 使った配列を開放 */
		delete[] pData;
		delete[] pHeights;
	}

};

/* read_obs.cpp */
std::string obs_path(std::string path, obsDateTime dt);
Observed* read_obs(int *aNobs, std::string path_obs);/*, int Nline);*/

#endif
