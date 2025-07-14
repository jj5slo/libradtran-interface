/*
 * FixedParams.h
 *
 * 地球・衛星軌道の諸元
 *
 */

class PlanetParam{/* 地球諸元（今の所半径だけ） */
private:
	const double p_radius;
public:
	PlanetParam(double radius) : p_radius(radius) {}

	double radius(void){ return p_radius; }
};

class SatelliteParam{/* 楕円軌道に拡張する際にupdate(時刻かなにか) で時刻を更新して、その時刻を元にlat, lonを計算して返すようにするとよさそう */
private:
	const double p_radius;/* 高度ではなく、半径。高度がほしいときはplanetparam.radius()を引き算してください */
	const double p_latitude;
	const double p_longitude;

public:
	SatelliteParam(double radius, double lat, double lon) : p_radius(radius), p_latitude(lat), p_longitude(lon) {}

	double radius(void){ return p_radius; }
	double latitude(void){ return p_latitude; }
	double longitude(void){ return p_longitude; }
};


	

