#pragma once
#include <vector>
#include <string>
#include "_Matrix.h"
#define _USE_MATH_DEFINES
#include "math.h"
#include <Eigen/Dense>
#include <ctime>

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
	int Gravity_compensation_Least(Grav s, std::vector<double> &pose, std::vector<double> &FT, std::vector<double> &result);

	Grav Ide_vLeast(std::vector<std::vector<double>> &FT, std::vector <std::vector<double>> &pose);

private:
	int rotvec2rotmat(double rx, double ry, double rz, double(*rotArray)[3]);

	int rpy2rotvec(double rx, double ry, double rz, double *rotvec);
};


