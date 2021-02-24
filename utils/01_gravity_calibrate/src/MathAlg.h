#pragma once
#include <vector>
#define _USE_MATH_DEFINES
#include "math.h"
#include "_Matrix.h"

class RountP
{
public:
	RountP();
	~RountP();
public:

	void slerp(double *p1, double *p2, double t, double *q);

	void quaternion2rot_vec(double *q, double *rot_vec);

	void quaternion2rpy(double *q, double *rot_vec);

	void quaternion2rpyxin(double *q, double *rot_vec);

	void LineSpeed(double(*Acqdata)[6], std::vector <std::vector<double>> &Pdata);

	int Circle(double(*Position)[6], double *cir_angle, double *cen_cir, double(*posetransmatrix)[4]);

	void rotvec2quaternion(double rx, double ry, double rz, double result[4]);

	void rpy2quaternion(double roll, double pitch, double yaw, double result[4]);

	int TSpeedCruveMethod_1(double q0, double q1, double v0, double v1, double vmax, double amax, double td, std::vector <std::vector<double>> &result, double *V_T);

	int TSSMethod_1(double(*Position)[6], std::vector <std::vector<double>> &Tread, double *T_V);

	int counter_BA(double *X, double *Y, double *Z, int row, double *b_x, double *b_y, double *b_z, double *U, double *w);

	int Bspline(double *X, double *Y, double *Z, double *U, int row, double u, double *Arr);

	int rpy2rotvec(double rx, double ry, double rz, double *rotvec);

	double polyVal(int row, double *K, double x0);

	int polyFit(std::vector<double> x, std::vector<double> y, int k, double *A);

	double TanVecB_basis(double u, double *U, int i);

	void TanVecBspline(int row, double *X, double *Y, double *Z, double *U, double u, double *result);

	double ComTanVecBspline(int row, double *X, double *Y, double *Z, double *U, double u);

	double ComputeArcLengthOfBspline(int row, double *X, double *Y, double *Z, double *U, double a, double b);

	int NURBSS(int row, double(*arr)[6], std::vector <std::vector<double>> &B_Arr);
};

