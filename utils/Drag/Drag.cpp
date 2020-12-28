#include "Drag.h"
#include <sys/stat.h>
#include <sys/types.h>

Drag::Drag()
{
    ThresData.push_back(2.0);
    ThresData.push_back(2.0);
    ThresData.push_back(2.0);
    ThresData.push_back(1.0);
    ThresData.push_back(1.0);
    ThresData.push_back(1.0);

    SensitiData.push_back(1.0);
    SensitiData.push_back(1.0);
    SensitiData.push_back(0.5);
    SensitiData.push_back(50.0);
    SensitiData.push_back(50.0);
    SensitiData.push_back(50.0);

    FdDrag.push_back(0.0);
    FdDrag.push_back(0.0);
    FdDrag.push_back(0.0);
    FdDrag.push_back(0.0);
    FdDrag.push_back(0.0);
    FdDrag.push_back(0.0);

    Aixcontrol.push_back(1.0);
    Aixcontrol.push_back(1.0);
    Aixcontrol.push_back(1.0);
    Aixcontrol.push_back(1.0);
    Aixcontrol.push_back(1.0);
    Aixcontrol.push_back(1.0);
    FilterN = 12;
    Filterflag = 1;
    FilterNum = 0;
    filehead = "/home/louwei/aubo_driver/Drag/data/";
    Filtercount = 0;
}
Drag::~Drag()
{

}
Grav Drag::read_fileide()
{
    Grav  cgs;

    char filename[100];
    char* fileend = "ideconfig.txt";
    //snprintf(filename,sizeof(filename),"%s%s",filehead,fileend);
    std::ifstream myfile(fileend);

    if (!myfile.is_open()) //判断如果文件指针为空
      {
            printf("File cannot open!");
            return cgs;//在以0的形式退出，必须在文件开头有#include <stdlib.h>,stdlib 头文件即standard library标准库头文件
      }
    else
      {
          myfile >> cgs.Fx0;
          myfile >> cgs.Fy0;
          myfile >> cgs.Fz0;
          myfile >> cgs.Tx0;
          myfile >> cgs.Ty0;
          myfile >> cgs.Tz0;
          myfile >> cgs.G;
          myfile >> cgs.Lx;
          myfile >> cgs.Ly;
          myfile >> cgs.Lz;
          myfile >> cgs.l1;
          myfile >> cgs.l2;
          myfile >> cgs.l3;
          myfile.close();
          return cgs;
      }
}
int Drag::printSend(std::vector<double> &pose,std::vector<double> &changepose,std::vector<double> &senddata)
{
    senddata.push_back(Aixcontrol[0]*changepose[0]);
    senddata.push_back(Aixcontrol[1]*changepose[1]);
    senddata.push_back(Aixcontrol[2]*changepose[2]);
    senddata.push_back(Aixcontrol[3]*changepose[3]);
    senddata.push_back(Aixcontrol[4]*changepose[4]);
    senddata.push_back(Aixcontrol[5]*changepose[5]);
    return 100;
}
/*****************************************************************************
*
* FUNCTION:			Filter
*
* DESCRIPTION:»¬¶¯ÆœŸùÖµÂË²š
*  This is the main program module.
*
\*****************************************************************************/
int Drag::Filter(const std::vector<double> &force, std::vector<double> &Fforce, int row)
{
    double sum[6] = { 0 };
    Fforce.clear();
    for (int i = 0; i < 6; i++)
    {
        Filterbuff[row][i] = force[i];
    }
    for (int j = 0; j < 6; j++)
    {
        for (int i = 0; i < FilterN; i++)
        {
            sum[j] += Filterbuff[i][j];
        }
    }
    for (int j = 0; j < 6; j++)
    {
        Fforce.push_back(sum[j] / FilterN);
    }
    return 0;
}
void Drag::Judg_Filter(const std::vector<double> &force, std::vector<double>&Fforce)
{
    std::vector<double> FFilter;
    FFilter.clear();
    Fforce.resize(6);
    if (Filterflag == 1)
    {
        if (FilterNum == 0)
        {
            Filter(force, FFilter, Filtercount);
            Filtercount++;
            for (int j = 0; j < 6; j++)
            {
                Fforce[j] = force[j];
            }
            if (Filtercount >= FilterN - 1)
            {
                FilterNum = 1;
            }
        }
        else
        {
            Filter(force, FFilter, Filtercount);
            Filtercount++;
            for (int j = 0; j < 6; j++)
            {
                Fforce[j] = FFilter[j];
            }
            if (Filtercount > FilterN)
            {
                Filtercount = 0;
            }
        }
    }
    else
    {
        for (int j = 0; j < 6; j++)
        {
            Fforce[j] = force[j];
        }
    }
}
/*****************************************************************************
*
* FUNCTION:			File_open
*
* DESCRIPTION:ÎÄŒþŽò¿ª¡£
*
*  This is the main program module.
*
\*****************************************************************************/
int Drag::File_open(std::ofstream& my, char *name)
{
    char *filename_t = File_NameC(name);
   int ret =  File_cread(name);
    //printf("%d \n",ret);
     std::cout<<filename_t<<std::endl;
    my.open(filename_t, std::ios::trunc);
    my.setf(std::ios::fixed, std::ios::floatfield);  // Éè¶šÎª fixed Ä£Êœ£¬ÒÔÐ¡Êýµã±íÊŸž¡µãÊý
    my.precision(6);  // ÉèÖÃŸ«¶È 6
    if (!my.is_open()) //ÅÐ¶ÏÈç¹ûÎÄŒþÖžÕëÎª¿Õ
    {
        printf("File cannot open!");
        return 0;//ÔÚÒÔ0µÄÐÎÊœÍË³ö£¬±ØÐëÔÚÎÄŒþ¿ªÍ·ÓÐ#include <stdlib.h>,stdlib Í·ÎÄŒþŒŽstandard library±ê×Œ¿âÍ·ÎÄŒþ
    }
    return 100;
}
/*****************************************************************************
*
* FUNCTION:			File_NameC
*
* DESCRIPTION:ÆŽœÓÎÄµµÃû×Ö·ûŽ®
*  This is the main program module.
*
\*****************************************************************************/
char* Drag::File_NameC(char* name)
{
    static char filename[150];
    char* fileDelimiter = "/";
    char* fileend = ".txt";
    char* name1 = Obtain_Current_Time(name);
    snprintf(filename, sizeof(filename), "%s%s%s%s%s", filehead,name, fileDelimiter, name1, fileend);
    return filename;
}
/*****************************************************************************
*
* FUNCTION:			Obtain_Current_Time
*
* DESCRIPTION:»ñÈ¡µ±Ç°ÏµÍ³Ê±Œä
*  This is the main program module.
*
\*****************************************************************************/
char* Drag::Obtain_Current_Time(char *name)
{
    time_t rawtime;
    struct tm *ptminfo;
    char s[50];
    strcpy(s, name);
    int len = strlen(s) + 17;
    static char res[50];
    time(&rawtime);
    ptminfo = localtime(&rawtime);
    /*printf("current: %02d-%02d-%02d %02d:%02d:%02d\n",
    ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
    ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);*/
    snprintf(res, len, "%s%02d-%02d-%02d-%02d-%02d-%02d\n",name,
        ptminfo->tm_year + 1900, ptminfo->tm_mon + 1, ptminfo->tm_mday,
        ptminfo->tm_hour, ptminfo->tm_min, ptminfo->tm_sec);
    printf("res£º%s\n", res);

    return res;
}
/*****************************************************************************
*
* FUNCTION:			File_cread
*
* DESCRIPTION:ŽŽœšÎÄŒþŒÐ¡£
*
*  This is the main program module.
*
\*****************************************************************************/
int Drag::File_cread(char *name)
{
    std::string folderPath = "/home/louwei/aubo_driver/Drag/data/";
    std::string file = folderPath + name;
    std::cout<<"file is:"<<file<<std::endl;
    //std::cout << file << std::endl;
    if (0 != access(file.c_str(), 0))//ÅÐ¶ÏÎÄŒþŒÐÊÇ·ñŽæÔÚ
    {
        // if this folder not exist, create a new one.
        if (0 == mkdir(file.c_str(), 777))   // ·µ»Ø 0 ±íÊŸŽŽœš³É¹Š£¬-1 ±íÊŸÊ§°Ü
        {
            return 100;								//»»³É ::_mkdir  ::_access Ò²ÐÐ£¬²»ÖªµÀÊ²ÃŽÒâËŒ
        }
        else {
            return 0;
        }
    }
    else
    {
        return 0;
    }
}
Limitdrive::Limitdrive()
{
    rangex = 0.2;
    rangey = 0.2;
    rangez = 0.2;
    limitF = 10;
    limitT = 3;
    initx = 0;
    inity = 0;
    initz = 0;
}
Limitdrive::~Limitdrive()
{

}
double Limitdrive::getrangex()
{
    return rangex;
}
double Limitdrive::getrangey()
{
    return rangey;
}
double Limitdrive::getrangez()
{
    return rangez;
}
double Limitdrive::getlimitF()
{
    return limitF;
}
double Limitdrive::getlimitT()
{
    return limitT;
}
void Limitdrive::setlimitF(double LF)
{
    limitF = LF;
}
void Limitdrive::setlimitT(double LT)
{
    limitT = LT;
}
void Limitdrive::setrangex(double x)
{
    rangex = x;
}
void Limitdrive::setrangey(double y)
{
    rangey = y;
}
void Limitdrive::setrangez(double z)
{
    rangez = z;
}
void Limitdrive::setinitpose(std::vector<double> &initpose)
{
    initx = initpose[0];
    inity = initpose[1];
    initz = initpose[2];
    initr = initpose[3];
    initp = initpose[4];
    initry = initpose[5];
}
void Limitdrive::rectangle(std::vector<double> &pose,std::vector<double> &changepose,std::vector<double> &Limitpose)
{
    Limitpose.clear();
    if((initx+rangex)<(pose[0]+changepose[0])||(initx-rangex)>(pose[0]+changepose[0]))
    {
        if(changepose[0]>=0)
        {
            Limitpose.push_back(-0.0005);
        }
        else
        {
            Limitpose.push_back(0.0005);
        }
    }
    else
    {
        Limitpose.push_back(changepose[0]);
    }
    if((inity+rangey)<(pose[1]+changepose[1])||(inity-rangey)>(pose[1]+changepose[1]))
    {
        if(changepose[1]>=0)
        {
            Limitpose.push_back(0.0005);
        }
        else
        {
            Limitpose.push_back(0.0005);
        }
    }
    else
    {
        Limitpose.push_back(changepose[1]);
    }
    if((initz+rangez)<(pose[2]+changepose[2])||(initz-rangez)>(pose[2]+changepose[2]))
    {
        if(changepose[2]>=0)
        {
            Limitpose.push_back(0.0005);
        }
        else
        {
            Limitpose.push_back(0.0005);
        }
    }
    else
    {
        Limitpose.push_back(changepose[2]);
    }
    Limitpose.push_back(initr);
    Limitpose.push_back(initp);
    Limitpose.push_back(initry);
}
