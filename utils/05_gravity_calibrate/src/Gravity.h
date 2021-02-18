#pragma once
#include <vector>
#include "_Matrix.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <Eigen/Dense>

struct Grav {
	double Fx0, Fy0, Fz0, Tx0, Ty0, Tz0, G, Lx, Ly, Lz;
	double l1, l2, l3;
};
class Gravity
{
public:
	Gravity();
	~Gravity();
public:
	int rotvec2rotmat(double rx, double ry, double rz, double(*rotArray)[3]);

	int rpy2rotvec(double rx, double ry, double rz, double *rotvec);

	int Gravity_compensation_Least(Grav s, double p[6], double sp[6], double result[6]);

	Grav Ide_vLeast(Grav s, std::vector<std::vector<double>> &vecser, std::vector <std::vector<double>> &vecur, int m);
};

