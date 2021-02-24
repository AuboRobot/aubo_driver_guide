#ifndef DRAG_H
#define DRAG_H
#include <vector>
#include "Gravity.h"
#include <iostream>
#include <fstream>
#include <sstream>

class     Drag
{
public:
    Drag();
    ~Drag();
public:
    std::vector<std::vector<double>> FTdata;
    std::vector<std::vector<double>> posedata;
    std::vector<double> FT;
    std::vector<double> pose;
public:
    Grav read_fileide();
    std::vector<double> ThresData;
    std::vector<double> SensitiData;
    std::vector<double> FdDrag;
    std::vector<double> Aixcontrol;
    int printSend(std::vector<double> &pose,std::vector<double> &changepose,std::vector<double> &senddata);
    int Filter(const std::vector<double> &force, std::vector<double> &Fforce, int row);
    void Judg_Filter(const std::vector<double> &force, std::vector<double>&Fforce);
private:
    double Filterbuff[128][6];
    int FilterN;
    int Filterflag;
    int FilterNum;
    double Data[6];
    int  Filtercount;
public:
    char* filehead;
    int File_open(std::ofstream& my, char *name);
    char* File_NameC(char* name);
    char *Obtain_Current_Time(char *name);
    int File_cread(char *name);

};
class   Limitdrive
{
public:
    Limitdrive();
    ~Limitdrive();
public:
    void rectangle(std::vector<double> &pose,std::vector<double> &changepose,std::vector<double> &Limitpose);
    void limitforce(std::vector<double> &force,std::vector<double> &Lforce);
public:
    double getrangex();
    double getrangey();
    double getrangez();
    double getlimitF();
    double getlimitT();
    void setrangex(double x);
    void setrangey(double y);
    void setrangez(double z);
    void setlimitF(double LF);
    void setlimitT(double LT);
    void setinitpose(std::vector<double> &initpose);
private:
    double rangex;
    double rangey;
    double rangez;
    double limitF;
    double limitT;
    double initx;
    double inity;
    double initz;
    double initr;
    double initp;
    double initry;
};

#endif // DRAG_H
