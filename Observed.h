#include<cmath>

#ifndef __OBSERVED_H__
#define __OBSERVED_H__

class Observed{/* ある方向の高度ごとのデータ */
private:
	double pLat;
	double pLon;
	double *pHeights;
	double *pData;
	int pNheights;
public:
	void set(double lat, double lon, int Nheights, double *heights, double *data);
	double Latitude(void);
	double Longitude(void);
	double* Heights(void);
	double Data(double height);
	int Nheights(void);

	~Observed(){/* 使った配列を開放 */
		delete[] pData;
		delete[] pHeights;
	}

};

#endif
