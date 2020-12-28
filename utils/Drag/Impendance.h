#pragma once
#include <stdio.h>
#include <string>
#include <ctime>
#include <cstring>
#include <vector>
class Impendance
{
public:
	Impendance();
	~Impendance();
public:
	int ImpendanceControl(double xr, double xr_dot, double xr_dotdot, double force, double * xcForRobot, int xrNum);

	int Impen_Inter(std::vector<double> &pose, std::vector<double> &ThresData,  std::vector<double> &SensitiData,std::vector<double> &FdDrag, std::vector<double> &Changepose);

	int ImpendanceControl_Surface(double xr, double xr_dot, double xr_dotdot, double force, double * xcForRobot, int xrNum);

private:
	double M;
	double B;
	double K_D;
	double INTEVAL;
	double P_inp;
	double t_f;

};

