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
	PlanetParam(double radius) : p_radius(radius) {}/* [m] */

	double radius(void){ return p_radius; }
};

class SatelliteParam{/* 楕円軌道に拡張する際にupdate(時刻かなにか) で時刻を更新して、その時刻を元にlat, lonを計算して返すようにするとよさそう */
private:
	double p_radius;/* [m] *//* 高度ではなく、半径。高度がほしいときはplanetparam.radius()を引き算してください */
	double p_latitude;
	double p_longitude;

public:
	SatelliteParam(void) : p_radius(42164.e3), p_latitude(0.0), p_longitude(0.0) {}
	SatelliteParam(double radius, double lat, double lon) : p_radius(radius), p_latitude(lat), p_longitude(lon) {}

	double radius(void){ return p_radius; }
	double latitude(void){ return p_latitude; }
	double longitude(void){ return p_longitude; }
};


	

